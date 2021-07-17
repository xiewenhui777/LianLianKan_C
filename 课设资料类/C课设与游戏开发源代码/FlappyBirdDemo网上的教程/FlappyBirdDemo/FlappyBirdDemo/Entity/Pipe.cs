using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Configuration;
using FlappyBirdDemo.Properties;

namespace FlappyBirdDemo.Entity
{
    /// <summary>
    /// 游戏对象二：管道
    /// </summary>
    public class Pipe : GameObject
    {
        public Pipe(int x, int y)
            : base(x, y)
        {
            this.Direction = PipeDirectionEnum.Up;
            this.Width = 135;
        }

        public Pipe(int x, int y, PipeDirectionEnum direction)
            : base(x, y)
        {
            this.Direction = direction;
            this.Width = 135;
        }

        public PipeDirectionEnum Direction
        {
            get;
            set;
        }

        // 获得管道的图片
        private Image pipeImage = Resources.flappy_packer;

        /// <summary>
        /// 从大图中根据横纵坐标获取具体管道
        /// </summary>
        /// <param name="x">X坐标</param>
        /// <param name="y">Y坐标</param>
        /// <returns>管道图片</returns>
        public Image GetImage(int x, int y)
        {
            // 创建位图：宽128，高830
            Bitmap bmp = new Bitmap(128, 830);
            // 将截取出的图片通过GDI+绘制到位图中
            Graphics g = Graphics.FromImage(bmp);
            // 开始绘制管道：单位为Pixel像素
            g.DrawImage(pipeImage, new Rectangle(0, 0, 128, 830),
                new Rectangle(x, y, 128, 830), GraphicsUnit.Pixel);
            // 参数1：new Rectangle(0, 0, 128, 830) 表示要绘制到bmp上的矩形大小
            // 参数2：new Rectangle(x, y, 128, 830) 表示从原图的哪个位置截取图片

            return bmp;
        }

        /// <summary>
        /// 绘制管道到窗体中
        /// </summary>
        /// <param name="g">GDI+绘图对象</param>
        public override void Draw(Graphics g)
        {
            switch (this.Direction)
            {
                case PipeDirectionEnum.Up:
                    g.DrawImage(GetImage(160, 495), this.X, this.Y);
                    break;
                case PipeDirectionEnum.Down:
                    g.DrawImage(GetImage(10, 459), this.X, this.Y);
                    break;
            }
        }

        /// <summary>
        /// 移动管道位置：右->左 移动
        /// </summary>
        public override void Move()
        {
            this.X -= 10;
        }
    }
}
