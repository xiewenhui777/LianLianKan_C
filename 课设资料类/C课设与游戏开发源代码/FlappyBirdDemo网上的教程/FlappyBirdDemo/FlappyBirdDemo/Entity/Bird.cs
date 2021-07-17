using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using FlappyBirdDemo.Properties;

namespace FlappyBirdDemo.Entity
{
    /// <summary>
    /// 游戏对象一：小鸟
    /// </summary>
    public class Bird : GameObject
    {
        // 小鸟图片数组
        private static Image[] images = 
        {
            Resources._0,
            Resources._1,
            Resources._2
        };

        // 小鸟图片的索引
        public int BirdIndex
        {
            get;
            set;
        }

        /// <summary>
        /// 小鸟当前的速度
        /// </summary>
        public float CurrentSpeed
        {
            get;
            set;
        }

        /// <summary>
        /// 当前获取时间
        /// </summary>
        public int DurationTime
        {
            get;
            set;
        }

        public Bird(int x, int y, int birdIndex)
            : base(x, y, images[0].Width, images[0].Height)
        {
            this.BirdIndex = birdIndex;
            this.DurationTime = 100;
            this.CurrentSpeed = 0f;
        }

        // 绘制小鸟到窗体中
        public override void Draw(Graphics g)
        {
            // 根据索引的不同绘制不同翅膀的小鸟图片
            switch (this.BirdIndex)
            {
                case 0:
                    g.DrawImage(images[1], this.X, this.Y);
                    break;
                case 1:
                    g.DrawImage(images[2], this.X, this.Y);
                    break;
                case 2:
                    g.DrawImage(images[0], this.X, this.Y);
                    break;
            }

            this.BirdIndex++;
            if (this.BirdIndex >= 3)
            {
                this.BirdIndex = 0;
            }
        }

        // 移动小鸟位置：上下移动
        public override void Move()
        {
            this.Y -= 40;
            // 避免小鸟移动位置超过游戏界面
            if (this.Y <= 0)
            {
                this.Y = 0;
            }
        }
    }
}
