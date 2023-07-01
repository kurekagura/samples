using App1.Shared;
using Microsoft.AspNetCore.Mvc;

namespace App1.Server.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class PieChartController : ControllerBase
    {
        private readonly ILogger<PieChartController> _logger;

        public PieChartController(ILogger<PieChartController> logger)
        {
            _logger = logger;
        }

        [HttpGet]
        public IEnumerable<Pie> Get()
        {
            var random = new Random();
            int a, b, c;

            do
            {
                a = random.Next(0, 101); // 0から100までのランダムな整数を生成
                b = random.Next(0, 101 - a); // a以下の範囲でランダムな整数を生成
                c = 100 - a - b; // a+b+c=100となるようにcを計算
            } while (c < 0 || c > 100); // cが0未満または100を超える場合、再生成

            return new List<Pie>
            {
                new Pie(){title="A", value = a},
                new Pie(){title="B", value = b},
                new Pie(){title="C", value = c}
            };
        }
    }
}
