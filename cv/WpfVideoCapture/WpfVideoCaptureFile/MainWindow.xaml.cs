using OpenCvSharp.WpfExtensions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using OpenCvSharp;
using System;

namespace WpfVideoCaptureFile
{
    public partial class MainWindow : System.Windows.Window
    {
        private string _inputFilePath = @"C:\dev\samplevideo\h264movie.mp4";
        private CancellationTokenSource _cancelTokenSource;

        public MainWindow()
        {
            InitializeComponent();
        }

        async private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                using (var vcap = new VideoCapture(_inputFilePath))
                {
                    if (!vcap.IsOpened())
                    {
                        MessageBox.Show("ファイルを開くことが出来ませんでした。アプリケーションを終了します。");  //AnyCPUはエラーになる場合有った。
                        this.Close();
                        return;
                    }

                    _cancelTokenSource = new CancellationTokenSource();
                    await Task.Run(() =>
                    {

                    top:
                        vcap.PosFrames = 0;
                        while (true)
                        {
                            if (_cancelTokenSource.Token.IsCancellationRequested)
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
                            //Task.Delay(30);
                            System.Threading.Thread.Sleep(30);
                        }

                        if (!_cancelTokenSource.Token.IsCancellationRequested)
                            goto top;

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
