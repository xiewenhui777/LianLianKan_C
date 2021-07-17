using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace FlappyBirdDemo.Entity
{
    /// <summary>
    /// 游戏对象单例模式类
    /// </summary>
    public class SingleObject
    {
        private SingleObject() { }

        private static SingleObject singleInstance;

        public static SingleObject GetInstance()
        {
            if (singleInstance == null)
            {
                singleInstance = new SingleObject();
            }
            return singleInstance;
        }

        public Bird SingleBird
        {
            get;
            set;
        }

        /// <summary>
        /// 添加游戏对象
        /// </summary>
        /// <param name="parentObject">游戏对象父类</param>
        public void AddGameObject(GameObject parentObject)
        {
            if(parentObject is Bird)
            {
                SingleBird = parentObject as Bird;
            }
        }

        /// <summary>
        /// 绘制游戏对象
        /// </summary>
        /// <param name="g"></param>
        public void DrawGameObject(Graphics g)
        {
            SingleBird.Draw(g);
        }
    }
}
