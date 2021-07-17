namespace FlappyBirdDemo
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.pbxGround = new System.Windows.Forms.PictureBox();
            this.BirdTimer = new System.Windows.Forms.Timer(this.components);
            this.GravityTimer = new System.Windows.Forms.Timer(this.components);
            this.PipeTimer = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pbxGround)).BeginInit();
            this.SuspendLayout();
            // 
            // pbxGround
            // 
            this.pbxGround.BackgroundImage = global::FlappyBirdDemo.Properties.Resources.ground;
            this.pbxGround.Location = new System.Drawing.Point(1, 542);
            this.pbxGround.Name = "pbxGround";
            this.pbxGround.Size = new System.Drawing.Size(369, 103);
            this.pbxGround.TabIndex = 0;
            this.pbxGround.TabStop = false;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(370, 644);
            this.Controls.Add(this.pbxGround);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(386, 682);
            this.MinimizeBox = false;
            this.MinimumSize = new System.Drawing.Size(386, 682);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Flappy Bird";
            ((System.ComponentModel.ISupportInitialize)(this.pbxGround)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pbxGround;
        private System.Windows.Forms.Timer BirdTimer;
        private System.Windows.Forms.Timer GravityTimer;
        private System.Windows.Forms.Timer PipeTimer;
    }
}

