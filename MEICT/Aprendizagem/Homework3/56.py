from sklearn import datasets
from sklearn.model_selection import train_test_split
import random
import numpy as np
import matplotlib.pyplot as plt

iris = datasets.load_iris()
X = iris.data[:, :2]  # we only take the first two features.
y = (iris["target"] == 2).astype(np.int)  # 1 if Iris-Virginica, else 0

# explicit function to normalize array
def normalize_2d(matrix):
    norm = np.linalg.norm(matrix)
    matrix = matrix / norm  # normalized matrix
    return matrix

normalized_matrix = normalize_2d(X)

# Split into a train and test partition.
X_train, X_test, y_train, y_test = train_test_split(normalized_matrix, y, test_size=0.2, random_state=0)
l_rate = 0.1
n_epoch = 30

# Configuration 1

# Make a prediction with weights
def predictLinear(X, weights, bias):
    prediction = np.dot(X, weights) + bias
    if prediction > 0.5:
        prediction = 1
    else:
        prediction = 0
    return prediction

# Estimate Perceptron weights using stochastic gradient descent
def train_weights_model1(X_train, lRate, nEpoch, nSamples, i):
    weights = [random.random() for i in range(len(X_train))]
    bias = random.random()
    for epoch in range(nEpoch):
        prediction = predictLinear(X_train, weights, bias)
        dw = (1 / nSamples) * X_train * (prediction - y_train[i])
        db = (1 / nSamples) * (prediction - y_train[i])
        weights = weights - lRate * dw
        bias = bias - lRate * db
    return weights, bias

# Perceptron Algorithm With Stochastic Gradient Descent
def perceptronModel1(X_train, X_test, l_rate, n_epoch):
    lRate = l_rate
    nEpoch = n_epoch
    nSamples, nFeatures = X_train.shape
    allWeights = []
    allBias = []
    for i in range(len(X_train)):
        weights, bias = train_weights_model1(X_train[i], lRate, nEpoch, nSamples, i)
        allWeights.append(weights)
        allBias.append(bias)
    predictions = list()
    for i in range(len(y_test)):
        pred = predictLinear(X_test[i], allWeights[i], allBias[i])
        predictions.append(pred)
    return predictions

y_predM1 = perceptronModel1(X_train, X_test, l_rate, n_epoch)
print("Prediction for Configuration 1 =", y_predM1)

def compute_squared_loss(y, y_pred):
    loss = 1 / 2 * np.mean((y_pred - y_test) ** 2)
    return loss

lossesM1 = []
loss = compute_squared_loss(y_test, y_predM1)
lossesM1.append(loss)
print("Loss for Configuration 1 =", loss)

num_correctM1train = 0
for i in range(len(y_predM1)):
    if y_predM1[i] == y_train[i]:
        num_correctM1train += 1
accTrainM1 = num_correctM1train / len(y_test)
print("Train accuracy for Configuration 1 =", accTrainM1)

num_correctM1test = 0
for i in range(len(y_predM1)):
    if y_predM1[i] == y_test[i]:
        num_correctM1test += 1
accTestM1 = num_correctM1test / len(y_test)
print("Test accuracy for Configuration 1 =", accTestM1)

epochsM1 = []
errorsM1 = []
for i in range(n_epoch):
    y_predCalc = perceptronModel1(X_train, X_test, l_rate, i)
    epochsM1.append(i)
    err = compute_squared_loss(y_train, y_predCalc)
    errorsM1.append(err)

plt.plot(epochsM1, errorsM1)
plt.xlabel('Epochs')
plt.ylabel('Error')
plt.title("Configuration 1")
plt.show()

# Configuration 2

n_epoch = 500

def sigmoid(z):
    return 1 / (1 + np.exp(-z))

def sigmoid_grad(z):
    return sigmoid(z) * (1 - sigmoid(z))

# Make a prediction with weights
def predictsigmoid(X, weights, bias):
    Z = np.dot(X, weights) + bias
    prediction = sigmoid(Z)
    if prediction > 0.5:
        prediction = 1
    else:
        prediction = 0
    return prediction

# Estimate Perceptron weights using stochastic gradient descent
def train_weights_model2(X_train, lRate, nEpoch, nSamples, i):
    weights = [random.random() for n in range(len(X_train))]
    bias = random.random()
    for epoch in range(nEpoch):
        prediction = predictsigmoid(X_train, weights, bias)
        dw = (2*(prediction - y_train[i])*sigmoid_grad(np.dot(X_train, weights) + bias)) * np.transpose(X_train)
        db = 2*(prediction - y_train[i])*sigmoid_grad(np.dot(X_train, weights) + bias)
        weights = weights - lRate * dw
        bias = bias - lRate * db
    return weights, bias

# Perceptron Algorithm With Stochastic Gradient Descent
def perceptronModel2(X_train, X_test, l_rate, n_epoch):
    lRate = l_rate
    nEpoch = n_epoch
    nSamples, nFeatures = X_train.shape
    allWeights = []
    allBias = []
    for i in range(len(X_train)):
        weights, bias = train_weights_model2(X_train[i], lRate, nEpoch, nSamples, i)
        allWeights.append(weights)
        allBias.append(bias)
    predictions = list()
    for i in range(len(y_test)):
        pred = predictsigmoid(X_test[i], allWeights[i], allBias[i])
        predictions.append(pred)
    return predictions

y_predM2 = perceptronModel2(X_train, X_test, l_rate, n_epoch)
print("Prediction for Configuration 2 =", y_predM2)

lossesM2 = []
lossM2 = compute_squared_loss(y_test, y_predM2)
lossesM2.append(lossM2)
print("Loss for Configuration 2 =", lossM2)

num_correctM2train = 0
for i in range(len(y_predM2)):
    if y_predM2[i] == y_train[i]:
        num_correctM2train += 1
accTrainM2 = num_correctM2train / len(y_test)
print("Train accuracy for Configuration 2 =", accTrainM2)

num_correctM2test = 0
for i in range(len(y_predM2)):
    if y_predM2[i] == y_test[i]:
        num_correctM2test += 1
accTestM2 = num_correctM2test / len(y_test)
print("Test accuracy for Configuration 2 =", accTestM2)


# Configuration 3

n_epoch = 1000

# Estimate Perceptron weights using stochastic gradient descent
def train_weights_model3(X_train, lRate, nEpoch, nSamples):
    weights = [random.random() for i in range(len(X_train))]
    bias = random.random()
    for epoch in range(nEpoch):
        prediction = predictsigmoid(X_train, weights, bias)
        dw = (1 / nSamples) * X_train * (prediction - y_train[i])
        db = (1 / nSamples) * (prediction - y_train[i])
        weights = weights - lRate * dw
        bias = bias - lRate * db
    return weights, bias

# Perceptron Algorithm With Stochastic Gradient Descent
def perceptronModel3(X_train, X_test, l_rate, n_epoch):
    lRate = l_rate
    nEpoch = n_epoch
    nSamples, nFeatures = X_train.shape
    allWeights = []
    allBias = []
    for i in range(len(X_train)):
        weights, bias = train_weights_model2(X_train[i], lRate, nEpoch, nSamples, i)
        allWeights.append(weights)
        allBias.append(bias)
    predictions = list()
    for i in range(len(y_test)):
        pred = predictsigmoid(X_test[i], allWeights[i], allBias[i])
        predictions.append(pred)
    return predictions, weights

y_predM3, weightsM3 = perceptronModel3(X_train, X_test, l_rate, n_epoch)
print("Prediction for Configuration 3 =", y_predM3)

def compute_cross_entropy_loss(X, y, weights):
    z = np.dot(X, weights)
    predict_1 = y * np.log(sigmoid(z))
    predict_0 = (1 - y) * np.log(1 - sigmoid(z))
    ce = -sum(predict_1 + predict_0) / len(X)
    return ce

lossesM3 = []
lossM3 = compute_cross_entropy_loss(X_test, y_test, weightsM3)
lossesM3.append(lossM3)
print("Loss for Configuration 3 =", lossM3)

num_correctM3train=0
for i in range(len(y_predM3)):
    if y_predM3[i] == y_train[i]:
        num_correctM3train += 1
acctrainM3 = num_correctM3train / len(y_test)
print("Train accuracy for Configuration 3 =", acctrainM3)

num_correctM3test=0
for i in range(len(y_predM3)):
    if y_predM3[i] == y_test[i]:
        num_correctM3test += 1
acctestM3 = num_correctM3test / len(y_test)
print("Test accuracy for Configuration 3 =", acctestM3)
