using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace drbm_c_sharp
{
    class Program
    {
        static void Main(string[] args)
        {
            // UnityJankenの引数: 何手前まで考慮するか?
            UnityJanken janken = new UnityJanken(5);
            string[] name = new string[] {"グー", "チョキ", "パー" };
            while(true)
            {
                Console.Write("手を選べ[0: グー, 1: チョキ, 3: パー]...");
                int my_no = int.Parse(Console.ReadLine());
                if (my_no < 0 || 2 < my_no) continue;

                int enemy_no = janken.game(my_no);
                string messege = enemy_no == janken.getWinPattern(my_no) ? "You Lose" : "";

                Console.WriteLine("You:" + name[my_no] + ", Enemy: " + name[enemy_no]);
                Console.WriteLine( messege );
                Console.WriteLine();
            }

        }
    }
}
