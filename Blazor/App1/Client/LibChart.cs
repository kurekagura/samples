namespace App1.Client
{
    public class ChartJSON
    {
        public string type { get; set; }
        public ChartData data { get; set; }
    }

    public class ChartData
    {
        public IEnumerable<string> labels { get; set; }
        public IEnumerable<ChartDataset> datasets { get; set; }
    }

    public class ChartDataset
    {
        public string label { get; set; }
        public IEnumerable<int> data { get; set; }
        public string backgroundColor { get; set; }
        public string borderColor { get; set; }
        public int borderWidth { get; set; }
    }
}
