using System;
using System.IO;
using OpenCvSharp;

namespace CvSharpUsage
{
    class Program
    {
        static void Main(string[] args)
        {
            ResizeSequentialBmpTo4K();
        }

        /// <summary>
        /// 連番BMPファイルをFHD，4Kなどに拡大する為に作成．
        /// </summary>
        static void ResizeSequentialBmpTo4K()
        {
            var bmpFilePathList = Directory.GetFiles("C:\\dev\\samplevideo\\out-avframe2mat");
            //var dstDirPath = "C:\\dev\\samplevideo\\out-avframe2mat-4k";
            var dstDirPath = "C:\\dev\\samplevideo\\out-avframe2mat-fhd";

            Directory.CreateDirectory(dstDirPath);

            foreach (var bmpFilePath in bmpFilePathList)
            {
                var src = Cv2.ImRead(bmpFilePath);
                var dst = new Mat();

                //var newSize = new Size(3840, 2160);
                var newSize = new Size(1920, 1080);

                Cv2.Resize(src, dst, newSize, 0, 0, InterpolationFlags.Nearest);

                var newFilePath = Path.Combine(dstDirPath, Path.GetFileName(bmpFilePath));

                if (!dst.SaveImage(newFilePath))
                    throw new Exception("Failed:SaveImage");
            }

        }
    }
}
