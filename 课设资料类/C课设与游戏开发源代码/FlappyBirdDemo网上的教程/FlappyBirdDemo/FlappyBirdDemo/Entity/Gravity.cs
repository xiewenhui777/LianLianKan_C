using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;

namespace FlappyBirdDemo.Entity
{
    /// <summary>
    /// 重力辅助类
    /// </summary>
    public class Gravity
    {
        public static float gravity = 9.8f;

        /// <summary>
        /// s = 1/2*gt^2+vt
        /// </summary>
        /// <param name="speed">速度</param>
        /// <param name="second">时间</param>
        /// <returns>位移量</returns>
        public static float GetHeight(float speed, float time)
        {
            float height = (float)(0.5 * gravity * time * time)
                + speed * time;
            return height;
        }
    }
}
