import nltk

class Analyzer():
    """Implements sentiment analysis."""

    # Stores positive/negative words in lists positives/negatives
    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = []
        self.negatives = []

        with open("positive-words.txt", "r") as pfile:
            for line in pfile:
                if not line.startswith(";"):
                    self.positives.append(line.strip())

        with open("negative-words.txt", "r") as nfile:
            for line in nfile:
                if not line.startswith(";"):
                    self.negatives.append(line.strip())

    # Compares input word with both sets, returning either 1(+ve), -1(-ve) or 0(neutral/not found)
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0

        tokenizer = nltk.tokenize.TweetTokenizer(False)
        tokens = tokenizer.tokenize(text)
        token = "".join(tokens)

        for tokenite in token.split():
            if tokenite in self.positives:
                score += 1
            elif tokenite in self.negatives:
                score -= 1
            else:
                continue

        return score
