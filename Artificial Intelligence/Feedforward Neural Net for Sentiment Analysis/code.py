#####----- 1. Get Data -----#####
import torch
from datasets import load_dataset
dataset = load_dataset("sst2")
origin_x = dataset['train']['sentence']
origin_test_x = [
    'it’s a charming and often affecting journey.',
    'unflinchingly bleak and desperate',
    'allows us to hope that nolan is poised to embark a major career as a commercial yet inventive filmmaker.',
    'the acting, costumes, music, cinematography and sound are all astounding given the production’s austere locales.',
    'it’s slow – very, very slow.',
    'although laced with humor and a few fanciful touches, the film is a refreshingly serious look at young women.',
    'a sometimes tedious film.',
    'or doing last year’s taxes with your ex-wife.',
    'you don’t have to know about music to appreciate the film’s easygoing blend of comedy and romance.',
    'in exactly 89 minutes, most of which passed as slowly as if i’d been sitting naked on an igloo, formula 51 sank from quirky to jerky to utter turkey.'
]
origin_x.extend(origin_test_x)
word_to_idx = {word: idx + 1 for idx, word in enumerate(set(word for sentence in origin_x for word in sentence.split()))}
tokenized_sentences = [[word_to_idx[word] for word in sentence.split()] for sentence in origin_x]
max_len = max(len(sentence) for sentence in tokenized_sentences)
padded_sentences = [sentence + [0] * (max_len - len(sentence)) for sentence in tokenized_sentences]
x_tensor = torch.tensor(padded_sentences, dtype=torch.long)
origin_x = torch.tensor(x_tensor, dtype=torch.float32)

# Train Data
x = origin_x[:-10, :]
y = torch.tensor(dataset['train']['label'], dtype=torch.float32)
y = y.unsqueeze(1)

# Test Data
test_x = origin_x[-10:, :]
test_y = torch.tensor([[1.0],[0.0],[1.0],[1.0],[0.0],[1.0],[0.0],[0.0],[1.0],[0.0]])

#####----- 2. Training -----#####
import torch.nn as nn
import torch.optim as optim

#Model
class MultilayerPerceptron(nn.Module):
  def __init__(self, input_size, hidden_size):
    super(MultilayerPerceptron, self).__init__()
    self.input_size = input_size
    self.hidden_size = hidden_size
    self.model = nn.Sequential(
        nn.Linear(self.input_size, self.hidden_size),
        nn.ReLU(),
        nn.Linear(self.hidden_size, self.hidden_size),
        nn.ReLU(),
        nn.Linear(self.hidden_size, 1),
        nn.Sigmoid()
    )
  def forward(self, x):
    output = self.model(x) 
    return output 
model = MultilayerPerceptron(52, 20)

#Optimization
adam = optim.Adam(model.parameters(), lr=0.0001)
loss_function = nn.BCELoss()
for epoch in range(700):
  adam.zero_grad()
  y_pred = model(x)
  loss = loss_function(y_pred, y)
  loss.backward()
  adam.step()
y_mean=y_pred.mean()
for i in range(len(y_pred)):
  if(y_pred[i]>y_mean):
    y_pred[i]=1
  else:
    y_pred[i]=0

#####----- 3. Testing -----#####
# Testing
test_y_pred = model(test_x)
test_y_mean=test_y_pred.mean()
for i in range(len(test_y_pred)):
  if(test_y_pred[i]>test_y_mean):
    test_y_pred[i]=1
  else:
    test_y_pred[i]=0

# Correctness
correct_count = 0
for i in range(len(test_y_pred)):
  if(test_y_pred[i]==test_y[i]):
    correct_count+=1
print("----------")
print("Ideal Result")
print(test_y)
print("----------")
print("Model Result")
print(test_y_pred)
print("----------")
print("Correctness")
print(correct_count/10)
print("----------")


