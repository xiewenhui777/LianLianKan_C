using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using FlappyBirdDemo.Entity;

namespace FlappyBirdDemo
{
    public partial class MainForm : Form
    {
        #region 00.主窗体构造函数
        public MainForm()
        {
            InitializeComponent();
            // 初始化游戏对象
            InitialGameObjects();
            // 初始化游戏事件
            InitialGameEvents();
        }
        #endregion

        #region 01.初始化游戏对象
        private Pipe pipeUp;
        private Pipe pipeDown;

        private void InitialGameObjects()
        {
            // 添加游戏对象 — 小鸟
            SingleObject.GetInstance().AddGameObject(new Bird(50, 200, 0));
            // 添加游戏对象 — 管道
            pipeUp = new Pipe(500, -600, PipeDirectionEnum.Up);
            pipeDown = new Pipe(500, 400, PipeDirectionEnum.Down);
        }
        #endregion

        #region 02.初始化游戏事件
        private void InitialGameEvents()
        {
            // 添加Form_Load事件
            this.Load += MainForm_Load;
            // 添加Form_Closed事件
            this.FormClosed += MainForm_FormClosed;
            // 添加Form_Paint事件
            this.Paint += MainForm_Paint;
            // 添加游戏对象Timer控件的Tick事件
            this.BirdTimer.Tick += BirdTimer_Tick;
            this.BirdTimer.Enabled = true;
            // 添加鼠标单击MouseDown事件
            this.MouseDown += MainForm_MouseDown;
            // 添加键盘单击KeyDown事件
            this.KeyDown += MainForm_KeyDown;
            // 添加重力Timer控件的Tick事件
            this.GravityTimer.Tick += GravityTimer_Tick;
            this.GravityTimer.Interval = 10;
            this.GravityTimer.Enabled = true;
            // 添加管道Timer控件的Tick事件
            this.PipeTimer.Tick += PipeTimer_Tick;
            this.PipeTimer.Interval = 10;
            this.PipeTimer.Enabled = true;
        }

        private void PipeTimer_Tick(object sender, EventArgs e)
        {
            this.MovePipeLine();
            // 碰撞检测
            Bird bird = SingleObject.GetInstance().SingleBird;
            if (bird.Y == 0 || bird.Y == this.pbxGround.Height ||
                bird.GetRectangeleArea()
                .IntersectsWith(pipeDown.GetRectangeleArea()) ||
                bird.GetRectangeleArea()
                .IntersectsWith(pipeUp.GetRectangeleArea()))
            {
                this.PauseGame();
                if (MessageBox.Show("您已挂了，是否购买王胖子的滑板鞋继续畅玩？",
                    "温馨提示", MessageBoxButtons.YesNo,
                    MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    this.InitialGameObjects();
                    this.RestoreGame();
                }
                else
                {
                    MessageBox.Show("您的选择是明智的，王胖子的滑板鞋太挫了！",
                        "温馨提示", MessageBoxButtons.OK, 
                        MessageBoxIcon.Information);
                    Environment.Exit(0);
                }
            }
        }

        private void PauseGame()
        {
            this.BirdTimer.Enabled = false;
            this.GravityTimer.Enabled = false;
            this.PipeTimer.Enabled = false;
        }

        private void RestoreGame()
        {
            this.BirdTimer.Enabled = true;
            this.GravityTimer.Enabled = true;
            this.PipeTimer.Enabled = true;
        }

        private void MovePipeLine()
        {
            // 从右至左移动管道
            pipeUp.Move();
            pipeDown.Move();
            // 当管道完全移出窗体时准备显示下一个管道
            if (pipeUp.X <= -128)
            {
                pipeUp.X = this.Width * 4 / 3 - 128;
                pipeDown.X = this.Width * 4 / 3 - 128;

                pipeUp.Height = GetRandomHeight();
                pipeDown.Height = this.Size.Height - pbxGround.Height
                    - pipeDistance - pipeUp.Height;

                //根据高度计算出Y轴值
                pipeUp.Y = pipeUp.Height - 830;
                pipeDown.Y = pipeUp.Height + pipeDistance;
            }
        }

        private int pipeDistance = 150;

        private int GetRandomHeight()
        {
            Random random = new Random();
            int totalHeight = this.Size.Height - this.pbxGround.Height;
            return random.Next(90, totalHeight - 90 - pipeDistance);
        }

        private void GravityTimer_Tick(object sender, EventArgs e)
        {
            Bird singleBird = SingleObject.GetInstance().SingleBird;
            // Step1:获得小鸟下降的高度
            float height = Gravity.GetHeight(singleBird.CurrentSpeed,
                singleBird.DurationTime * 0.001f);
            // singleBird.DurationTime * 0.001f => 将毫秒转换成帧
            // Step2:获得小鸟下落后的坐标
            int y = singleBird.Y + (int)height;
            // Step3:将新Y轴坐标赋给小鸟
            int min = this.Size.Height - this.pbxGround.Height
                - 60;
            if (y > min)
            {
                // 限定小鸟不要落到地面下
                y = min;
            }
            singleBird.Y = y;
            // Step4:使小鸟按照加速度下降 [ 公式：v=v0+at ]
            singleBird.CurrentSpeed = singleBird.CurrentSpeed
                + Gravity.gravity * singleBird.DurationTime * 0.001f;
        }

        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            // 如果用户敲的是Space键
            if (e.KeyCode == Keys.Space)
            {
                Bird bird = SingleObject.GetInstance().SingleBird;
                // 使小鸟向上移动
                bird.Move();
                bird.CurrentSpeed = 10f;
            }
        }

        private void MainForm_MouseDown(object sender, MouseEventArgs e)
        {
            Bird bird = SingleObject.GetInstance().SingleBird;
            // 使小鸟向上移动
            bird.Move();
            bird.CurrentSpeed = 10f;
        }

        private void BirdTimer_Tick(object sender, EventArgs e)
        {
            // 使控件的整个图像无效并导致重绘控件
            this.Invalidate();
        }

        private void MainForm_Paint(object sender, PaintEventArgs e)
        {
            // 绘制游戏对象 — 小鸟
            SingleObject.GetInstance().DrawGameObject(e.Graphics);
            // 绘制游戏对象 — 管道
            pipeUp.Draw(e.Graphics);
            pipeDown.Draw(e.Graphics);
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            // 终止游戏进程
            Environment.Exit(0);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            // To do some initial welcome tips
        }
        #endregion
    }
}
