namespace AoC2024;

partial class Form1
{
    /// <summary>
    ///  Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    ///  Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
        if (disposing && (components != null))
        {
            components.Dispose();
        }

        base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
        terminal = new System.Windows.Forms.RichTextBox();
        SuspendLayout();
        // 
        // terminal
        // 
        terminal.BackColor = System.Drawing.Color.Black;
        terminal.Dock = System.Windows.Forms.DockStyle.Fill;
        terminal.Font = new System.Drawing.Font("Iosevka Term Semibold", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte) 0));
        terminal.ForeColor = System.Drawing.Color.LimeGreen;
        terminal.Location = new System.Drawing.Point(0, 0);
        terminal.Name = "terminal";
        terminal.ReadOnly = true;
        terminal.Size = new System.Drawing.Size(800, 450);
        terminal.TabIndex = 0;
        terminal.Text = "";
        // 
        // Form1
        // 
        AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
        AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        ClientSize = new System.Drawing.Size(800, 450);
        Controls.Add(terminal);
        Text = "Form1";
        ResumeLayout(false);
    }

    private System.Windows.Forms.RichTextBox terminal;

    #endregion
}