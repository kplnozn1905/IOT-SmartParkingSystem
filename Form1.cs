using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
//Derleyiciye giriş çıkış portlarını kullanacağımızı bildiriyoruz.
//We're telling the compiler that we're going to use the I/O ports
using System.IO.Ports;

namespace Csharp_MühTas1
{
    public partial class Form1 : Form
    {
        //Serial portlardan gelen verileri üzerine yazdığımız string değerler
        // String values that we overwrite the data coming from the serial ports
        private string LcdIR;
        private string RfidIR;

        //cmbEntrySpeed de seçilecek değerlerin isimleri.
        //Names of the values to be selected in cmbEntrySpeed.
        public string[] barrierspeed = { "FAST", "MIDDLE", "SLOW" };

        //Sistem kullanıcı bilgilerii tuttuğumuz model
        // Model where we keep system user information
        public class Users
        {
            public string UserId { get; set; }
            public string CardId { get; set; }
            public string UserName { get; set; }
            public int UserBalance { get; set; }

        }
        //Kullanıcıların tanımlanması
        //Defining users
        Users user1 = new Users() { UserId = "1", UserName = "Ozan Hamdi Kaplan", UserBalance = 50, CardId = "29" };
        Users user2 = new Users() { UserId = "2", UserName = "Eray Kaplan", UserBalance = 300, CardId = "13" };
        Users user3 = new Users() { UserId = "3", UserName = "Merve Şahin", UserBalance = 0 , CardId = "47" };
        Users user4 = new Users() { UserId = "4", UserName = "Sibel Şafak", UserBalance = 10, CardId = "19" };
        Users user5 = new Users() { UserId = "5", UserName = "Ayşe Serim", UserBalance = 30,CardId = "41" };

        //Kullanıcıların saklanacağı liste oluşturuldu
        // Created a list of users
        List<Users> NewUsers = new List<Users>(4);

        public int x;
        public int i=0;
        public int y;
        public int balance;

        public Form1()
        {
            InitializeComponent();
        }

        //Uygulama kapatma
        // Application shutdown
        private void pBClose_Click(object sender, EventArgs e)
        {
            spParkLcd.Close();
            spRfidEntryExit.Close();
            spCarLamp.Close();
            System.Windows.Forms.Application.Exit();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

            //Listeye kullanıcılar eklendi
            // Added users to the list
            NewUsers.Add(user1);
            NewUsers.Add(user2);
            NewUsers.Add(user3);
            NewUsers.Add(user4);
            NewUsers.Add(user5);

            //ListView tablo ayarları yapıldı
            //ListView table settings are made
            lstvUser.GridLines = true;
            lstvUser.FullRowSelect = true;
            lstvUser.View = View.Details;

            lstvUser.Columns.Add("ID");
            lstvUser.Columns.Add("USER NAME");
            lstvUser.Columns.Add("BALANCE");
            lstvUser.Columns[0].Width = 50;
            lstvUser.Columns[1].Width = 180;
            lstvUser.Columns[2].Width = 110;
            //Bakiye yükleme sisteminde kullanıcı seçilecek olan combobox indexlerine listelenen kullanıcıların isimleri atandı
            //Names of the listed users are assigned to the combobox indexes that will be selected in the balance loading system.
            foreach (Users c in NewUsers)
            {
                string[] userinfo = new string[3] { c.UserId,c.UserName, c.UserBalance.ToString() };
                string[] cmb = new string[1] { c.UserName.ToString() };
                cmbUSER.Items.AddRange(cmb);
                ListViewItem lst = new ListViewItem(userinfo);
                lstvUser.Items.Add(lst);

            }

            //ComboBoxların ilk gözükecek textler ayarlandı
            // The first texts to appear in ComboBoxes are set
            cmbUSER.Text = "Please Select User!!!";
            cmbEntrySpeed.Text = "Please Select Speed!!!";
            cmbExitSpeed.Text = "Please Select Speed!!!";

            //Tanımlanan barrierspeed matrix değerleri combobox indexleriyle eşleştirildi
            // Defined barrier speed matrix values matched with combobox indexes
            cmbEntrySpeed.Items.AddRange(barrierspeed);
            cmbExitSpeed.Items.AddRange(barrierspeed);



            //Serial Port ayarları ve açma işlemleri yapıldı
            //Serial Port settings and opening operations are done
            spParkLcd.PortName = "COM13";
            spRfidEntryExit.PortName = "COM3";
            spCarLamp.PortName = "COM9";
            try
            {
                //Veri gönderip alabilmek için seri portumuzu açıyoruz
                // We open our serial port to send and receive data
                spParkLcd.Open();
                spRfidEntryExit.Open();
                spCarLamp.Open();
            }
            catch(Exception ex) { MessageBox.Show(ex.Message); }

            //Seri port datareceived ile dinleniyor
            // Listening to serial port datareceived
            spParkLcd.DataReceived += new SerialDataReceivedEventHandler(SpParkLcd_DataReceived);
            spRfidEntryExit.DataReceived += new SerialDataReceivedEventHandler(SpRfidEntryExit_DataReceived);

            //Araç farı kapalı şekilde başlatıldı
            //Initialized with vehicle headlight off
            spCarLamp.Write("6");
            PbLampOFF.Visible = true;
            PbLampLOW.Visible = false;
            PbLampMIDDLE.Visible = false;
            PbLampHIGH.Visible = false;

            
            btn4Park_Click(this, EventArgs.Empty);

            //Seri porta veri gönderildi
            //Data sent to the serial port
            spParkLcd.Write("x");
        }
        //Seri porttan gelen verileri okuyup tanımladığımız değişkene aktarıyoruz
        // We read the data from the serial port and transfer it to the variable we defined
        private void SpParkLcd_DataReceived(object sender,SerialDataReceivedEventArgs e)
        {
            LcdIR = spParkLcd.ReadExisting();
            this.Invoke(new EventHandler(DisplayDataPark_event));
        }

        //Veri alındıktan sonra yapılacaklar 
        // What to do after data is received
        private void DisplayDataPark_event(object sender,EventArgs e)
        {

            LblLCDSb.Text = LcdIR;
            //Park Yerlerinin dolu ya da boş olma durumları
            // Whether the parking spaces are full or empty
            switch (LblLCDSb.Text)
            {

                case "A":
                    pbP1.BackColor = Color.Chartreuse;
                    btn3Park.Enabled = true;
                    break;
                case "B":
                    pbP1.BackColor = Color.DarkRed;
                    btn3Park.Enabled = false;
                    break;
                case "C":
                    pbP2.BackColor = Color.Chartreuse;
                     break;
                case "D":
                    pbP2.BackColor = Color.DarkRed;
                      break;
                case "E":
                    pbP3.BackColor = Color.Chartreuse;
                    break;
                case "F":
                    pbP3.BackColor = Color.DarkRed;
                    break;
                case "G":
                    pbP4.BackColor = Color.Chartreuse;
                    break;
                case "H":
                    pbP4.BackColor = Color.DarkRed;
                    break;

            }
            //Park yerleri 1 veya 2 durumlarına göre btn2 ve btn3 kullanılabilirliği 
            // Availability of btn2 and btn3 based on parking spots 1 or 2
            if (pbP1.BackColor ==Color.DarkRed || pbP2.BackColor == Color.DarkRed)
            {
                btn2Park.Enabled = false;
            }
            else
            {
                btn2Park.Enabled = true;
            }
            if (pbP1.BackColor == Color.DarkRed || pbP2.BackColor == Color.DarkRed)
            {
                btn2Park.Enabled = false;
            }
            else
            {
                btn2Park.Enabled = true;
            }
        }
        private void SpRfidEntryExit_DataReceived(object sender,SerialDataReceivedEventArgs e)
        {
            RfidIR = spRfidEntryExit.ReadExisting();
            this.Invoke(new EventHandler(DisplayDataRfid_event));
        }
        private void DisplayDataRfid_event(object sender,EventArgs e)
        {
            LblRfidSp.Text = RfidIR;

            switch (LblRfidSp.Text)
            {
                case "a":

                    //Boş park alanı olmadığında yapılacaklar ayarlandı
                    // Set what to do when there is no free parking space
                    if (!(pbP1.BackColor == Color.Chartreuse || pbP2.BackColor == Color.Chartreuse || pbP3.BackColor == Color.Chartreuse || pbP4.BackColor == Color.Chartreuse))
                    {
                        MessageBox.Show("Sorry Parking spaces are full..", "Error!!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        spParkLcd.Write("y");
                    }
                    spParkLcd.Write("a");
                    
                    break;

                case "b":
                    //Çıkışta araç var
                    //There is a vehicle at the exit
                    if (i == 0)
                    {
                        spParkLcd.Write("b");
                        MessageBox.Show("There is a vehicle at the exit" + "\n"+"OPEN THE BARRIER", "İNFO!!!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        i= 1;
                    }
                    
                    break;
                
                default:

                    spParkLcd.Write("c");
                    CardID.Text = LblRfidSp.Text;
                    //Kullanıcı  bakiye kontrolü
                    // User balance check
                    CheckBalance(CardID.Text.ToString());
                    break;
            }
                    

        }

        #region Balance Check Function
        public void CheckBalance(string cardID)
        {
            foreach (Users c in NewUsers)
            {

                if (cardID == c.CardId.ToString())
                {
                    //Yetersiz bakiye
                    // Insufficient balance
                    if (c.UserBalance < 10)
                    {
                        MessageBox.Show("Inefficient Fund Please Recharge..", "Error!!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        spParkLcd.Write("d");
                    }
                    else
                    {
                        c.UserBalance -= 10;
                        MessageBox.Show("Balance payment successfully completed", "İNFO", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        spParkLcd.Write("e");
                    }
                }
            }
                        
            lstvUser.Items.Clear();
            foreach (Users c in NewUsers)
            {
                string[] userinfo = new string[3] { c.UserId, c.UserName, c.UserBalance.ToString() };

                ListViewItem lst = new ListViewItem(userinfo);
                lstvUser.Items.Add(lst);
            }
        }
        #endregion

        #region Car Light Control
        private void swCarLamp_OnValueChange_1(object sender, EventArgs e)
        {   
            if(swCarLamp.Value == false)
            {
                PbLampOFF.Visible = true;
                PbLampLOW.Visible = false; 
                PbLampMIDDLE.Visible = false;
                PbLampHIGH.Visible = false;
                TrcBarCarLamp.Value = 0;
                spCarLamp.Write("6");
            }
            else {
            TrcBarCarLamp.Value =34;
            PbLampOFF.Visible=false;
            PbLampLOW.Visible = true;
                spCarLamp.Write("5");
            }
        }

        private void TrcBarCarLamp_ValueChanged(object sender, EventArgs e)
        {
            if (TrcBarCarLamp.Value ==0)
            {
                swCarLamp.Value = false;
                PbLampOFF.Visible = true;
                PbLampLOW.Visible = false;
                PbLampMIDDLE.Visible = false;
                PbLampHIGH.Visible = false;
                spCarLamp.Write("6");
            }
            else if (TrcBarCarLamp.Value == 34)
            {
                swCarLamp.Value = true;
                PbLampOFF.Visible = false;
                PbLampLOW.Visible = true;
                PbLampMIDDLE.Visible = false;
                PbLampHIGH.Visible = false;
                spCarLamp.Write("5");
            }
            else if (TrcBarCarLamp.Value == 64)
            {
                PbLampOFF.Visible = false;
                PbLampLOW.Visible = false;
                PbLampMIDDLE.Visible = true;
                PbLampHIGH.Visible = false;
                spCarLamp.Write("4");
            }
            else if (TrcBarCarLamp.Value == 100)
            {
                PbLampOFF.Visible = false;
                PbLampLOW.Visible = false;
                PbLampMIDDLE.Visible = false;
                PbLampHIGH.Visible = true;
                spCarLamp.Write("3");
            }

            
        }
        #endregion

        #region Balance Load System
        private void btnLoadbalance_Click(object sender, EventArgs e)
        {
                if (rdb10Balance.Checked == true)
                {
                    balance = int.Parse(rdb10Balance.Text); x = 1;
                }
                else if (rdb20Balance.Checked == true)
                {
                    balance = int.Parse(rdb20Balance.Text); x = 1;
                }
                else if (rdb30Balance.Checked == true)
                {
                    balance = int.Parse(rdb30Balance.Text); x = 1;
                }
                else
                {
                    x = 0;
                }
                foreach (Users c in NewUsers)
                {
                    if (cmbUSER.SelectedItem.ToString() == c.UserName.ToString())
                    {
                        c.UserBalance += balance;
                    }
                }
                lstvUser.Items.Clear();
                foreach (Users c in NewUsers)
                {
                    string[] userinfo = new string[3] { c.UserId, c.UserName, c.UserBalance.ToString() };
                    ListViewItem lst = new ListViewItem(userinfo);
                    lstvUser.Items.Add(lst);
                }
                if (x == 1)
                {
                    MessageBox.Show("Balance upload successfully completed", "İNFO", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    rdb10Balance.Checked = false;
                    rdb20Balance.Checked = false;
                    rdb30Balance.Checked = false;
                    cmbUSER.Text = "Please Select User!!!";
                }
                else
                {
                    MessageBox.Show("Please select the amount you want to top up!!!", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
        }
        #endregion
         
        #region Barrier Control 
        private void btnEntryopenclose_Click(object sender, EventArgs e)
        {
            if (cmbEntrySpeed.SelectedIndex == -1)
            {
                MessageBox.Show("Please select barrier speed!!!", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                spRfidEntryExit.Write(cmbEntrySpeed.SelectedIndex.ToString());
                spParkLcd.Write("x");
            }
        }

        private void btnExitopenclose_Click(object sender, EventArgs e)
        {
            if (cmbExitSpeed.SelectedIndex == -1)
            {
                MessageBox.Show("Please select barrier speed!!!", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else
            {
                spParkLcd.Write(cmbExitSpeed.SelectedIndex.ToString());
                spParkLcd.Write("x");
            }
            if (i == 1) { i = 2; }
            else { i = 0; }
        }
        #endregion

        #region Determining The Number of Parking Spaces
        private void btn4Park_Click(object sender, EventArgs e)
        {
            spParkLcd.Write("A");
            btn4Park.IdleFillColor = Color.Orange;
            btn4Park.IdleForecolor = Color.Black;
            btn4Park.IdleLineColor = Color.Orange;
            btn2Park.IdleFillColor = Color.Transparent;
            btn2Park.IdleForecolor = Color.FromArgb(176, 222, 180);
            btn2Park.IdleLineColor = Color.FromArgb(176, 222, 180);
            btn3Park.IdleFillColor = Color.Transparent;
            btn3Park.IdleForecolor = Color.FromArgb(176, 222, 180);
            btn3Park.IdleLineColor = Color.FromArgb(176, 222, 180);
        }

        private void btn3Park_Click(object sender, EventArgs e)
        {
            spParkLcd.Write("B");
            btn3Park.IdleFillColor = Color.Orange;
            btn3Park.IdleForecolor = Color.Black;
            btn3Park.IdleLineColor = Color.Orange;
            pbP1.BackColor = Color.DarkBlue;
            btn2Park.IdleFillColor = Color.Transparent;
            btn2Park.IdleForecolor = Color.FromArgb(176, 222, 180);
            btn2Park.IdleLineColor = Color.FromArgb(176, 222, 180);
            btn4Park.IdleFillColor = Color.Transparent;
            btn4Park.IdleForecolor = Color.FromArgb(176, 222, 180);
            btn4Park.IdleLineColor = Color.FromArgb(176, 222, 180);
        }

        private void btn2Park_Click(object sender, EventArgs e)
        {
            spParkLcd.Write("C");
            btn2Park.IdleFillColor = Color.Orange;
            btn2Park.IdleForecolor = Color.Black;
            btn2Park.IdleLineColor = Color.Orange;
            pbP1.BackColor = Color.DarkBlue;
            pbP2.BackColor = Color.DarkBlue;
            btn3Park.IdleFillColor = Color.Transparent;
            btn3Park.IdleForecolor = Color.FromArgb(176, 222, 180);
            btn3Park.IdleLineColor = Color.FromArgb(176, 222, 180);
            btn4Park.IdleFillColor = Color.Transparent;
            btn4Park.IdleForecolor = Color.FromArgb(176, 222, 180);
            btn4Park.IdleLineColor = Color.FromArgb(176, 222, 180);
        }
        #endregion
                    
    }
}
                        
