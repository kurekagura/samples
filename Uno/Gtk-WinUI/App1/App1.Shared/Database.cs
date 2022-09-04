using Microsoft.EntityFrameworkCore;
using System.ComponentModel.DataAnnotations;

namespace App1
{
    public sealed class DatabaseContext : DbContext
    {
        public DatabaseContext(string databasePath)
        {
            _dbPath = databasePath;
        }

        private readonly string _dbPath;

        protected override void OnConfiguring(DbContextOptionsBuilder options)
        {
            options.UseSqlite($"Data Source={_dbPath}");
        }

        public DbSet<Product> Products { get; set; } = default!;
    }

    public class Product
    {
        public Product()
        {

        }

        public Product(string name, string mediaPath)
        {
            Name = name;
            MediaPath = mediaPath;
        }

        public Product(int id, string name, string mediaPath)
        {
            Id = id;
            Name = name;
            MediaPath = mediaPath;
        }

        [Key]
        public int Id { get; set; }
        public string Name { get; set; }
        public string MediaPath { get; set; } //TODO:Tooltip
    }
}
