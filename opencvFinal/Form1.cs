using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;
using System.IO;

namespace opencvFinal
{
    public partial class Form1 : Form
    {
        private static string globalFileName = string.Empty;
        private static string hash = string.Empty;
        private string directoryPath = string.Empty;

        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern IntPtr extractWatermark(string imagePath, int len);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern IntPtr calculateImageHash(string imagePath);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void embedWatermark(string imagePath, string watermarkText);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applyNegative(string imagePath, string outputPath);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applyBlur(string imagePath, string outputPath);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applyEdgeDetection(string imagePath, string outputPath);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applyHistogramEqualization(string imagePath, string outputPath);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applySaltPepperNoise(string imagePath, string outputPath, float noiseRatio);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applyGrayscale(string imagePath, string outputPath);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applySharpen(string imagePath, string outputPath);
        [DllImport("C:\\Users\\lijim\\Desktop\\c++\\opencv\\opencvFinal\\x64\\Debug\\Dll1.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        static extern void applyContrastEnhancement(string imagePath, string outputPath);

        public Form1()
        {
            InitializeComponent();

        }

        private void openImage(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Title = "選擇圖片";
                openFileDialog.Filter = "圖片檔案 (*.jpg;*.jpeg;*.png;*.bmp)|*.jpg;*.jpeg;*.png;*.bmp|所有檔案 (*.*)|*.*";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
                        pictureBox1.Image = Image.FromFile(openFileDialog.FileName);
                        globalFileName = openFileDialog.FileName;
                        directoryPath = Path.GetDirectoryName(globalFileName);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("無法載入圖片: " + ex.Message);
                    }
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            IntPtr resultPtr = calculateImageHash(globalFileName);
            hash = Marshal.PtrToStringAnsi(resultPtr);
            label1.Text = hash;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            embedWatermark(globalFileName, hash);
            string outputPath = Path.Combine(directoryPath, "hash.png");
            pictureBox1.Image = Image.FromFile(outputPath);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            IntPtr Ptr = extractWatermark(Path.Combine(directoryPath, "hash.png"), hash.Length);
            if (Ptr != IntPtr.Zero)
            {
                string st = Marshal.PtrToStringAnsi(Ptr);
                MessageBox.Show("Length of extracted watermark: " + st.Length.ToString());
                label1.Text =  st.Length > 100 ? "讀取:"+st.Substring(0, 100) + "..." : "讀取:" + st;
            }
            else
            {
                MessageBox.Show("Failed to extract watermark.");
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            string userInput = textBox1.Text;
            hash += userInput;
            textBox1.Text = "";
            label1.Text = hash;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            label1.Text = "";
            hash = string.Empty;
        }

        private void applyImageEffect(Action<string, string> effectFunc, string effectName)
        {
            string outputPath = Path.Combine(directoryPath, $"{effectName}.png");
            effectFunc(globalFileName, outputPath);
            pictureBox1.Image = Image.FromFile(outputPath);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            applyImageEffect(applyNegative, "negative");
        }

        private void button8_Click(object sender, EventArgs e)
        {
            applyImageEffect(applyBlur, "blur");
        }

        private void button9_Click(object sender, EventArgs e)
        {
            applyImageEffect(applyEdgeDetection, "edgeDetection");
        }

        private void button10_Click(object sender, EventArgs e)
        {
            applyImageEffect(applyHistogramEqualization, "histogramEqualization");
        }

        private void button11_Click(object sender, EventArgs e)
        {
            string outputPath = Path.Combine(directoryPath, "saltPepper.png");
            applySaltPepperNoise(globalFileName, outputPath, 0.50f);
            pictureBox1.Image = Image.FromFile(outputPath);
        }

        private void button12_Click(object sender, EventArgs e)
        {
            applyImageEffect(applyGrayscale, "grayscale");
        }

        private void button13_Click(object sender, EventArgs e)
        {
            applyImageEffect(applySharpen, "sharpen");
        }

        private void button14_Click(object sender, EventArgs e)
        {
            applyImageEffect(applyContrastEnhancement, "contrastEnhanced");
        }
    }
}
