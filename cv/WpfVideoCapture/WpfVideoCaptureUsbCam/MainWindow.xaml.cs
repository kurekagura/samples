using OpenCvSharp;
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
            //await Task.Run(() =>
            //{
            //    using (var camera = new VideoCapture(0))
            //    using (var mat = new Mat())
            //    {
            //        while (true)
            //        {
            //            if (this._isStopped)
            //            {
            //                this.Dispatcher.Invoke(() => this.im.Source = null);
            //                break;
            //            }

            //            camera.Read(mat);

            //            if (mat.Empty())
            //                break;

            //            this.Dispatcher.Invoke(() =>
            //            {
            //                this.im.Source = mat.ToWriteableBitmap();
            //            });
            //        }
            //    }
            //});

            try
            {
                _cancelTokenSource = new CancellationTokenSource();
                using (var camera = new VideoCapture(0))
                {
                    var isLoop = true;
                    while (isLoop)
                    {
                        using (var mat = new Mat())
                        {
                            await Task.Run(() =>
                            {
                                try
                                {
                                    _cancelTokenSource.Token.ThrowIfCancellationRequested();
                                    camera.Read(mat);
                                    return mat;
                                }
                                catch (OperationCanceledException opex)
                                {
                                    System.Diagnostics.Debug.WriteLine(opex.ToString());
                                    return null;
                                }
                            })
                            .ContinueWith((t, status) =>
                            {
                                if (t.Result == null)
                                    isLoop = false; //キャンセルされた
                                else if (t.Result.Empty())
                                {
                                    //何らかの理由でEmpty:真っ黒？
                                    this.im.Source = t.Result.ToWriteableBitmap();
                                }
                                else
                                {
                                    this.im.Source = t.Result.ToWriteableBitmap();
                                }
                            }, _cancelTokenSource.Token, //指定するとTaskが実行されていない場合、即停止する。
                            TaskScheduler.FromCurrentSynchronizationContext());
                        }
                    }
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
