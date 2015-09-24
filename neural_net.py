import numpy as np
import matplotlib.pyplot as plt


def reLu(X):
	return (X>0)*X

def reLuGrad(X):
	return (X>0).astype(float)

def tanh(X): 
	return np.tanh(X)

def tanhGrad(X):
	return 1.0/(np.square(np.cosh(X)))


def init_two_layer_model(input_size, hidden_size, output_size):
  """
  Initialize the weights and biases for a two-layer fully connected neural
  network. The net has an input dimension of D, a hidden layer dimension of H,
  and performs classification over C classes. Weights are initialized to small
  random values and biases are initialized to zero.

  Inputs:
  - input_size: The dimension D of the input data
  - hidden_size: The number of neurons H in the hidden layer
  - ouput_size: The number of classes C

  Returns:
  A dictionary mapping parameter names to arrays of parameter values. It has
  the following keys:
  - W1: First layer weights; has shape (D, H)
  - b1: First layer biases; has shape (H,)
  - W2: Second layer weights; has shape (H, C)
  - b2: Second layer biases; has shape (C,)
  """
  # initialize a model
  model = {}
  weight_scale=5e-2
 # model['W1'] = 0.00001 * np.random.randn(input_size, hidden_size)
  model['W1'] = weight_scale * np.random.randn(input_size, hidden_size)
  model['b1'] = np.zeros(hidden_size)
  #model['W2'] = 0.00001 * np.random.randn(hidden_size, output_size)
  model['W2'] = weight_scale * np.random.randn(hidden_size, output_size)
  model['b2'] = np.zeros(output_size)
  return model


def two_layer_net(X, model, y=None, reg=0.0, reg2=0.0):
  
  """
  Compute the loss and gradients for a two layer fully connected neural network.
  The net has an input dimension of D, a hidden layer dimension of H, and
  performs classification over C classes. We use a softmax loss function and L2
  regularization the the weight matrices. The two layer net should use a ReLU
  nonlinearity after the first affine layer.

  Inputs:
  - X: Input data of shape (N, D). Each X[i] is a training sample.
  - model: Dictionary mapping parameter names to arrays of parameter values.
    It should contain the following:
    - W1: First layer weights; has shape (D, H)
    - b1: First layer biases; has shape (H,)
    - W2: Second layer weights; has shape (H, C)
    - b2: Second layer biases; has shape (C,)
  - y: Vector of training labels. y[i] is the label for X[i], and each y[i] is
    an integer in the range 0 <= y[i] < C. This parameter is optional; if it
    is not passed then we only return scores, and if it is passed then we
    instead return the loss and gradients.
  - reg: Regularization strength.

  Returns:
  If y not is passed, return a matrix scores of shape (N, C) where scores[i, c]
  is the score for class c on input X[i].

  If y is not passed, instead return a tuple of:
  - loss: Loss (data loss and regularization loss) for this batch of training
    samples.
  - grads: Dictionary mapping parameter names to gradients of those parameters
    with respect to the loss function. This should have the same keys as model.
  """

  # unpack variables from the model dictionary
  W1,b1,W2,b2 = model['W1'], model['b1'], model['W2'], model['b2']
  N, D = X.shape

  # compute the forward pass
  scores = None
  input1 = np.dot(X,W1)
  a_out1 = reLu(input1+b1)
  scores = (np.dot(a_out1,W2) +b2)

  
  # If the targets are not given then jump out, we're done
  if y is None:
    return scores

  # compute the loss
  loss = None
  
  snum = np.exp(scores)
  sden = np.sum(snum,1)

  import pdb

  smxloss = np.mean(-np.log(((snum[range(scores.shape[0]),y])/sden)))
  loss =  smxloss + .5*(reg*np.sum(W1*W1) + reg*np.sum(W2*W2)) #loss function of softmax
  
  
  
  # compute the gradients
  grads = {}
  
  dscores = snum/sden[:,None]    
  dscores[np.arange(scores.shape[0]),y] -=1
  dscores/=X.shape[0]           #normalize the scores
  #backprop for finding the differetial of scores
  
  dOut = np.dot(dscores,W2.T) #output differential of linear classifier
  dW2 = np.dot(a_out1.T,dscores) + reg*W2 +reg2*np.abs(W2) #weights in layer 2
  db2 = np.sum(dscores,axis=0) # biases of layer 2
  dbak = np.dot(dscores,W2.T)*reLuGrad(a_out1) #chain rule backwards
  dW1 = np.dot(X.T,dbak) +reg*W1 +reg2*np.abs(W1) #weights of layer 1
  db1 = np.sum(dbak,axis=0) #biases of layer 1

  
  grads["W2"] = dW2 # update the dictionary
  grads["W1"] = dW1
  grads["b1"] = db1
  grads["b2"] = db2
  

  return loss, grads


