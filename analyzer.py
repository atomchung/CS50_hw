import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # TODO
        # 創造 set 的資料結構
        self.positives = set()
        self.negatives = set()
        
        p_file = open(positives, "r")
        n_file = open(negatives, "r")
        
        # add 是資料結構 set 中 新增加元素的方法
        # 因為一個單字一行，所以直接一行一行讀，但是把最後面的 \n 拿掉
        for line in p_file:
            # 不要讀到開頭的註解 (用開頭是否是分號來區隔)
            if line.startswith(";") == False: 
                self.positives.add(line.rstrip("\n"))
        p_file.close()        
        
        for line in n_file:
            if line.startswith(";") == False:
                self.negatives.add(line.rstrip("\n"))
        n_file.close
        
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # TODO
        self.text = text
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        ## 初始分數值
        score = 0
        
        for c in tokens:
            ## 將文字 string 轉小寫
            temp = str.lower(c)
            if temp in self.positives:
                score = score + 1
            elif temp in self.negatives:
                score = score - 1
        return(score)
