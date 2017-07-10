using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace drbm_c_sharp
{
    public struct JankenStatus
    {
        public double tryNum;
        public double loseNum;

        public JankenStatus(double p1, double p2)
        {
            tryNum = p1;
            loseNum = p2;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            // UnityJankenの引数: 何手前まで考慮するか?
            UnityJanken janken = new UnityJanken(5);  // 5手前まで考慮するAI
            string[] name = new string[] {"グー", "チョキ", "パー" };

            JankenStatus status = new JankenStatus();

            int count = 100;

            while(0 < count)
            {
                int my_no = 0;
                Console.Write("手を選べ[1: グー, 2: チョキ, 3: パー]...");
                Console.Write("(残り" + count + "回)->");

                try
                {
                    my_no = int.Parse(Console.ReadLine()) - 1;
                    if (my_no < 0 || 3 < my_no) throw new Exception();
                } catch
                {
                    continue;
                }

                int enemy_no = janken.game(my_no);
                bool is_lose = enemy_no == janken.getWinPattern(my_no);
                string messege = is_lose ? "You Lose" : "";
                status.tryNum++;
                status.loseNum += is_lose ? 1 : 0;


                Console.WriteLine("You:" + name[my_no] + ", Enemy: " + name[enemy_no]);
                Console.WriteLine( messege );
                showLoseRate(ref status);

                count--;
            }

            while(true) {
                Console.ReadLine();
                showLoseRate(ref status);
            }
        }

        static void showLoseRate(ref JankenStatus status)
        {
            Console.WriteLine("敗率: " + status.loseNum / status.tryNum);
        }
    }
}
