from pathlib import Path
import shutil
import filecmp
from fnmatch import fnmatch

# 获取当前脚本所在目录的文件夹名
project_name = Path(__file__).resolve().parent.name

# 自动生成两个目录
A = Path(r"C:\Users\Pudge\source\repos") / project_name
if not A.is_dir():
    A = Path(r"C:\Users\Pudge\source\repos\Misc") / project_name
B = Path(r"D:\Github\Repositories") / project_name

print("项目名称：", project_name)
print("源目录 A：", A)
print("目标目录 B：", B)

# 相对于 B 的路径：这些文件或目录永远保留
KEEP = {
    ".vs",
    ".git",
    ".gitignore",
    "*.q",
    "*.md",
    "*.py",
    "*.jpg",
    "*.jpeg",
    "*.png",
}

# 先设为 True，只预览操作，不实际修改文件
DRY_RUN = False


def is_kept(relative_path: str) -> bool:
    path = Path(relative_path)
    name = path.name

    for pattern in KEEP:
        # 匹配任意目录层级中的文件名
        if fnmatch(name.lower(), pattern.lower()):
            return True

        # 精确路径匹配，以及保留目录下的所有内容
        keep_path = Path(pattern)

        if relative_path.lower() == pattern.lower():
            return True

        if keep_path in path.parents:
            return True

    return False


def sync():
    if not A.is_dir():
        raise RuntimeError(f"源目录不存在：{A}")

    if not B.is_dir():
        raise RuntimeError(f"目标目录不存在：{B}")

    # 复制 A 中的文件到 B，覆盖同名文件
    for src in A.rglob("*"):
        rel = src.relative_to(A)
        dst = B / rel

        if src.is_symlink():
            print(f"跳过符号链接：{rel}")
            continue

        if src.is_dir():
            if not dst.exists() and not DRY_RUN:
                dst.mkdir(parents=True, exist_ok=True)
            continue

        if src.is_file():
            # 不覆盖 B 独有的保留文件
            if is_kept(rel.as_posix()) and dst.exists():
                continue

            if (
                not dst.exists()
                or not dst.is_file()
                or not filecmp.cmp(src, dst, shallow=False)
            ):
                print(f"复制/更新：{rel}")
                if not DRY_RUN:
                    dst.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(src, dst)

    # 删除 B 中 A 不存在的文件
    for dst in sorted(B.rglob("*"), reverse=True):
        rel = dst.relative_to(B)

        if dst.is_symlink():
            continue

        if is_kept(rel.as_posix()):
            continue

        src = A / rel

        if dst.is_file() and not src.is_file():
            print(f"删除文件：{rel}")
            if not DRY_RUN:
                dst.unlink()

        elif dst.is_dir() and not src.is_dir():
            # 目录内只要还有保留文件，就不会删除该目录
            if any(dst.iterdir()):
                continue
            print(f"删除空目录：{rel}")
            if not DRY_RUN:
                dst.rmdir()

    print("\n预览完成。" if DRY_RUN else "\n同步完成。")


if __name__ == "__main__":
    sync()