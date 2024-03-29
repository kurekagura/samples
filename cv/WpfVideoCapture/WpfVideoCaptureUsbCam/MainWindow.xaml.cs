﻿using OpenCvSharp;
using OpenCvSharp.WpfExtensions;
using System;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace WpfVideoCaptureUsbCam
{
    public partial class MainWindow : System.Windows.Window
    {
        private CancellationTokenSource _cancelTokenSource;

        public MainWindow()
        {
            InitializeComponent();
        }

        async private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                using (var vcap = new VideoCapture(0))
                {
                    if (!vcap.IsOpened())
                    {
                        MessageBox.Show("USBカメラの認識に失敗しました。アプリケーションを終了します。");
                        this.Close();
                        return;
                    }

                    _cancelTokenSource = new CancellationTokenSource();
                    await Task.Run(() =>
                    {
                        while (true)
                        {
                            if (_cancelTokenSource.IsCancellationRequested)
                                break;

                            using (var mat = new Mat())
                            {
                                if (!vcap.Read(mat))
                                    break;

                                Dispatcher.Invoke(() =>
                                {
                                    im.Source = mat.ToWriteableBitmap();
                                });
                            }
                        }

                    }, _cancelTokenSource.Token);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"予期せぬ例外が発生しました。アプリケーションを終了します。{Environment.NewLine}" +
                    $"{ex.Message}{Environment.NewLine}" +
                    $"{ex.ToString()}");
                this.Close();
            }
            finally
            {
                _cancelTokenSource?.Dispose();
                _cancelTokenSource = null;
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _cancelTokenSource?.Cancel();
        }
    }
}
