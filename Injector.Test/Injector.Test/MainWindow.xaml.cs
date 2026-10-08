using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

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

        private void Button_Inject32_Click(object sender, RoutedEventArgs e)
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

        private void Button_Inject64_Click(object sender, RoutedEventArgs e)
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

        private void Button_InjectWithEvent32_Click(object sender, RoutedEventArgs e)//InjectWithEvent32
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

        private void Button_InjectWithEvent64_Click(object sender, RoutedEventArgs e)//InjectWithEvent64
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

        private void Button_Native32_Click(object sender, RoutedEventArgs e)
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

        private void Button_Native64_Click(object sender, RoutedEventArgs e)
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
