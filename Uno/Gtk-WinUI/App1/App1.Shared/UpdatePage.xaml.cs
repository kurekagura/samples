using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

namespace App1
{
    public sealed partial class UpdatePage : Page
    {
        public UpdatePage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            var selectedProduct = (Product)e.Parameter;
            grProduct.DataContext = selectedProduct;
        }

        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var selectedProduct = (Product)grProduct.DataContext;

                using (var dbctx = new DatabaseContext(App.DB_FILE_PATH))
                {
                    dbctx.Products.Update(selectedProduct);
                    dbctx.SaveChanges();

                    this.Frame.Navigate(typeof(MainPage));
                }
            }
            catch
            {

            }
        }

        async private void Picker_Click(object sender, RoutedEventArgs e)
        {
            var path = await Util.PickSingleFileAsync();
            if(!string.IsNullOrEmpty(path))
            {
                //var selectedProduct = (Product)spProduct.DataContext;
                //selectedProduct.MediaPath = path;
                tbMediaPath.Text = path;
            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(MainPage));
        }
    }
}
