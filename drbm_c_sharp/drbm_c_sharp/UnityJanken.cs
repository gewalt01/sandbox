﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace drbm_c_sharp
{
    class UnityJanken
    {
        public int historySize;  // 何手前まで考慮する?(|可視変数|= historySize * 4)
        public Queue<int> history = new Queue<int>();  // 相手の手前まで覚えておく? (訓練データの数)
        public int maxDataSize = 10; //さらに何件分まで訓練データとして使う?
        public DRBM drbm;
        public int hiddenSize = 3;
        public int batchSize = 10;  // とりあえず1, データ少ないとき適宜対応
        public double learningRate = 0.2;  // とりあえず0.2
        public int epoch = 10;  // とりあえず10回


        public UnityJanken(int histry_size)
        {
            this.historySize = histry_size;

            // 可視変数: {不明, グー, チョキ, パー}^history_size -> 4 * history_size
            // 隠れ変数: とりあえず50
            // 出力は{グー, チョキ, パー}の3種類
            this.drbm = new DRBM(4 * histry_size, 50, 3);
        }

        // class_no: 相手の新しく出した手
        public void train(int class_no)
        {
            List<List<double>> dataset = new List<List<double>>(this.history.Count);  // 

            for (int i = history.Count - 1; 0 <= i; i--)
            {
                var older_data = makeInputOneOfKFromHistory(i);
                dataset.Add(older_data);
            }


            // 勝てる手のラベルセット作成
            List<int> label = new List<int>(this.history.Count);
            //そっから過去のはT+1に勝てる手と対応づける
            for (int i = 0; i < history.Count - 1; i++)
            {
                //勝てる手は?
                var hist = this.history.ToList();
                int tmp_win_no = this.getWinPattern(hist[i+1]);
                label.Add(tmp_win_no);
            }

            if (0 < history.Count)
            {
                //最新履歴は今回出された手に勝つ
                label.Add(this.getWinPattern(class_no));
            }


            this.drbm.train(ref dataset, ref label, this.batchSize, this.learningRate, this.epoch);

            // 現在の履歴で学習
            if (this.maxDataSize < this.history.Count) history.Dequeue();
            history.Enqueue(class_no);

        }

        //  入力に対して勝てる手を決める(後だしじゃんけん)
        public int getWinPattern(int class_no)
        {
            // {グー, チョキ, パー} = {0, 1, 2}
            // 勝つには{パー, グー, チョキ} = {2, 0, 1}
            int[] pattern = new int[3] { 2, 0, 1 };

            return pattern[class_no];
        }

        //  勝負
        public int game(int class_no)
        {
            //  出す手を決める
            var no = this.inference();

            //  相手の手を見て学習
            this.train(class_no);

            return no;
        }

        //  クラス番号をOne-of-K表現に
        public List<double> toOneOfK(int class_no)
        {
            var ook = (new double[4]).ToList();
            ook[class_no + 1] = 1.0;

            return ook;
        }

        //  One-of-Kをクラス番号に
        List<int> toClassNo(ref List<double> ook)
        {
            List<int> class_list = new List<int>();

            for (int i = 0; i < ook.Count; i += 4)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (Math.Abs(ook[4 * i + j] - 1.0) < 0.00001)
                    {
                        class_list.Add(j);
                        break;
                    }

                    class_list.Add(-1);
                }
            }

            return class_list;
        }

        //  相手に勝てそうな手の予想
        public int inference()
        {
            List<double> data = makeInputOneOfKFromHistory(0);

            return drbm.discriminate(ref data);
        }

        public List<double> makeInputOneOfKFromHistory(int newer)
        {
            // 古いのを前に格納せよ
            var data = (new double[4 * this.historySize]).ToList();// 学習データ

            // 古い順に取得
            for (int i = 0; i < historySize; i++)
            {
                var ook = makeOneOfKFromHistory(newer + (historySize - 1 - i));

                for (int k = 0; k < 4; k++)
                {
                    data[i * 4 + k] = ook[k];  //  順番大丈夫?
                }
            }

            /*
            //for(int j = i; j < history.Count && j < historySize; j++)
            for (int j = s - (historySize - 1); j < history.Count && j < historySize; j++)
            {
                if (j < 0) continue;

                var ook = makeOneOfKFromHistory(j);

                for (int k = 0; k < 4; k++)
                {
                    data[(historySize - 1 - j) * 4 + k] = ook[k];  //  順番大丈夫?
                }
            }
            */

            return data;
        }


        public List<double> makeOneOfKFromHistory(int newer)
        {
            List<double> ook = (new double[4]).ToList();

            var tmp_hist = history.ToList();

            var index = history.Count - 1 - newer;
            if (index < 0) return ook;

            var class_no = tmp_hist[index];

            for (int i = 0; i < 4; i++)
            {
                if (i == class_no)
                {
                    ook[i] = 1.0;
                }

            }

            return ook;
        }
    }
}
