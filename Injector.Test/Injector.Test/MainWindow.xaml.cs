using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Injector.Test
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("Injector.Core.X64.dll")]
        extern static bool Inject(int PID, [MarshalAs(UnmanagedType.LPWStr)] string dllPath);
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            CLR.Injector.Console();

            //Inject(Process.GetProcessesByName("Client64")[0].Id, "HiJack64.dll");

            CLR.Injector.Inject(Process.GetProcessesByName("Client64")[0].Id, "HiJack64.dll");


            //CLR.Injector.Inject(Process.GetProcessesByName("Client32")[0].Id, "HiJack32.dll");

        }
    }
}
