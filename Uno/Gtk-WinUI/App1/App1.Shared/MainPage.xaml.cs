using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media.Imaging;
using System.Linq;
using Windows.Storage;
using System;

namespace App1
{
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
            updateViewFromDb();
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(AddPage));
        }

        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            var selectedProduct = this.lvProducts.SelectedItem as Product;
            if (selectedProduct != null)
            {
                this.Frame.Navigate(typeof(UpdatePage), selectedProduct);
            }
        }

        private void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var selectedProduct = this.lvProducts.SelectedItem as Product;
                if (selectedProduct != null)
                {
                    using (var db = new DatabaseContext(App.DB_FILE_PATH))
                    {
                        db.Products.Remove(selectedProduct);
                        db.SaveChanges();
                    }
                    updateViewFromDb();
                }
            }
            catch
            {

            }
        }

        private void updateViewFromDb()
        {
            using (var dbctx = new DatabaseContext(App.DB_FILE_PATH))
            {
                dbctx.Database.EnsureCreated();
                var data = dbctx.Products.ToList();

                lvProducts.ItemsSource = data;
            }
        }

        async private void SelectionChanged(object sender, RoutedEventArgs e)
        {
            var lv = (ListView)sender;
            var prod = (Product)lv.SelectedValue;

            //WriteableBitmap wb = new WriteableBitmap(100,100);
            if (!string.IsNullOrEmpty(prod.MediaPath))
            {
                var sf = await StorageFile.GetFileFromPathAsync(prod.MediaPath);
                using (var s = await sf.OpenReadAsync())
                {
                    var bitmap = new BitmapImage();
                    bitmap.SetSource(s);
                    this.imMedia.Source = bitmap;
                }
            }
        }
    }
}
