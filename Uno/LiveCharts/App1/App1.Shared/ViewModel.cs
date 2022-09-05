using LiveChartsCore;
using LiveChartsCore.SkiaSharpView;

namespace App1
{
    public class ViewModel
    {
        public ISeries[] Series { get; set; }

        public ViewModel()
        {
            var ls = new LineSeries<double>() { Values = new double[] { 2, 1, 3, 5, 3, 4, 6 } };
            Series = new ISeries[] { ls };
        }
    }
}
