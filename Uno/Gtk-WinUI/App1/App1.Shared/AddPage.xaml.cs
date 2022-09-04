using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

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
                var name = tbProductName.Text;
                using (var dbctx = new DatabaseContext(App.DB_FILE_PATH))
                {
                    var row = new Product(name);
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
    }
}
