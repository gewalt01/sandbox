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
            UnityJanken janken = new UnityJanken(5);
            int i = 0;
            var rand = new System.Random();
            while(true)
            {
                i = (i + 1)  % 3;
                int my_no = i;
//                int my_no = int.Parse(Console.ReadLine());
                int unity_no = janken.game(my_no);
                Console.WriteLine("I:" + my_no + ", Unity: " + unity_no + ", UnityWin:" + (unity_no == janken.getWinPattern(my_no)));
            }
            
        }
    }
}
