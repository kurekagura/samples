using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using Windows.Foundation.Metadata;
using Windows.Storage;
using Windows.Storage.Pickers;
using Microsoft.Win32;

namespace App1
{
    public sealed partial class AddPage : Page
    {
        public AddPage()
        {
            this.InitializeComponent();
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                //TODO:バインディングの利用
                var name = tbProductName.Text;
                if (string.IsNullOrEmpty(name))
                    throw new Exception();

                var mediaPath = string.IsNullOrEmpty(tbMediaPath.Text) ? null : tbMediaPath.Text;
                using (var dbctx = new DatabaseContext(App.DB_FILE_PATH))
                {
                    var row = new Product(name, mediaPath);
                    dbctx.Products.Add(row);
                    dbctx.SaveChanges();

                    this.Frame.Navigate(typeof(MainPage));
                }
            }
            catch
            {

            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(MainPage));
        }

        async private void Picker_Click(object sender, RoutedEventArgs e)
        {
            var path = await Util.PickSingleFileAsync();
            if (!string.IsNullOrEmpty(path))
            {
                tbMediaPath.Text = path;
                tbProductName.Text = System.IO.Path.GetFileNameWithoutExtension(path);
            }
        }
    }
}
