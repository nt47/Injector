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

            CLR.Injector.Console();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                CLR.Injector.Inject(Process.GetProcessesByName("Client32")[0].Id, "E:\\Injector\\HiJack32.dll");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }


        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            try
            {
                CLR.Injector.Inject(Process.GetProcessesByName("Client64")[0].Id, "E:\\Injector\\HiJack64.dll");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            try
            {
                Inject(Process.GetProcessesByName("Client32")[0].Id, "E:\\Injector\\HiJack32.dll");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)//InjectWithEvent32
        {
            try
            {
                CLR.Injector.InjectWithEvent(Process.GetProcessesByName("Client32")[0].Id, "E:\\Injector\\HiJack32e.dll", "#002");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)//InjectWithEvent64
        {
            try
            {
                CLR.Injector.InjectWithEvent(Process.GetProcessesByName("Client64")[0].Id, "E:\\Injector\\HiJack64e.dll", "#002");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            try
            {
                Inject(Process.GetProcessesByName("Client64")[0].Id, "E:\\Injector\\HiJack64.dll");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
