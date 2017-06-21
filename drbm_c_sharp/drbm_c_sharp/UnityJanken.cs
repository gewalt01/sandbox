using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace drbm_c_sharp
{
    class UnityJanken
    {
        public int historySize;  // 何手前まで考慮する?
        public List<int> history;  // 相手の手の履歴 FIXME: さらに何件分まで訓練データとして使う?
        public int tryCount = 0;  // 試行回数
        public DRBM drbm;
        public int hiddenSize = 50;
        public int batchSize = 5;  // とりあえず1, データ少ないとき適宜対応
        public double learningRate = 0.1;  // とりあえず0,1
        public int epoch = 100;  // とりあえず100回


        public UnityJanken(int histry_size)
        {
            this.historySize = histry_size;
            this.history = new List<int>(histry_size);

            // 可視変数: {不明, グー, チョキ, パー}^history_size -> 4 * history_size
            // 隠れ変数: とりあえず50
            // 出力は{グー, チョキ, パー}の3種類
            this.drbm = new DRBM(4 * histry_size, 50, 3);
        }

        // class_no: 相手の出した手
        public void train(int class_no)
        {
            // 相手の出し手に対して勝てる手は?
            int win_no = this.getWinPattern(class_no);

            // 現在の履歴で学習
            // FIXME: 何件分のデータで学習するの?

            // DRBM学習用データセット作成
            List<List<double>> dataset;

            // 勝てる手のラベルセット作成
            List<List<double>> label;


            this.drbm.train(ref dataset, ref label, this.batchSize, this.learningRate, this.epoch);
        }
        
        // 入力に対して勝てる手を決める(後だしじゃんけん)
        public int getWinPattern(int class_no)
        {
            // {グー, チョキ, パー} = {0, 1, 2}
            // 勝つには{パー, グー, チョキ} = {2, 0, 1}
            int[] pattern = new int[3] { 2, 0, 1 };

            return pattern[class_no];
        }

        // 相手に勝てそうな手の予想
        public int inference()
        {
            List<double> data = new List<double>(4 * this.historySize);  // DRBM入力用
            // DRBMように可視変数設計

            return drbm.discriminate(ref data);
        }
    }
}
