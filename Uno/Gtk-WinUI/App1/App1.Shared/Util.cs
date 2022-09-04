using System;
using System.Threading.Tasks;
using Windows.Foundation.Metadata;
using Windows.Storage.Pickers;

namespace App1
{
    public static class Util
    {
        public async static Task<string> PickSingleFileAsync()
        {
            if (ApiInformation.IsTypePresent("Windows.Storage.Pickers.FileOpenPicker"))
            {

                var ofpicker = new FileOpenPicker();
                //var casted = (IInitializeWithWindow)ofpicker;
                ofpicker.ViewMode = PickerViewMode.Thumbnail;
                ofpicker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
                //ofpicker.FileTypeFilter.Add("*");
                ofpicker.FileTypeFilter.Add(".jpg");
                ofpicker.FileTypeFilter.Add(".jpeg");
                ofpicker.FileTypeFilter.Add(".png");

                //↓WORKAROUND
#if NET6_0_OR_GREATER && WINDOWS && !HAS_UNO
                //var window = Microsoft.UI.Xaml.Window.Current; //->null
                var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(App.Window);
                WinRT.Interop.InitializeWithWindow.Initialize(ofpicker, hwnd);
#endif
                //↑WORKAROUND

                var storageFile = await ofpicker.PickSingleFileAsync();
                return storageFile?.Path;
            }
            else
                throw new NotImplementedException();

            //WPF
            //using Microsoft.Win32;
            //OpenFileDialog openFileDialog = new OpenFileDialog();
            //if (openFileDialog.ShowDialog() == true)
            //{

            //}

        }
    }
}
