using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace FlappyBirdDemo.Entity
{
    /// <summary>
    /// 游戏对象基类
    /// </summary>
    public abstract class GameObject
    {
        #region 01.构造函数及属性
        public int X { get; set; }

        public int Y { get; set; }

        public int Width { get; set; }

        public int Height { get; set; }

        public GameObject(int x, int y)
        {
            this.X = x;
            this.Y = y;

            this.Width = this.Height = 0;
        }

        public GameObject(int x, int y, int width, int height)
        {
            this.X = x;
            this.Y = y;
            this.Width = width;
            this.Height = height;
        } 
        #endregion

        #region 02.抽象方法
        /// <summary>
        /// 抽象方法1：绘制自身
        /// </summary>
        public abstract void Draw(Graphics g);

        /// <summary>
        /// 抽象方法2：移动自身
        /// </summary>
        public abstract void Move(); 
        #endregion

        #region 03.实例方法
        public Rectangle GetRectangeleArea()
        {
            return new Rectangle(this.X, this.Y, this.Width, this.Height);
        } 
        #endregion
    }
}
