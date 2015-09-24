import numpy as np
from random import shuffle

def svm_loss_naive(W, X, y, reg):
  """
  Structured SVM loss function, naive implementation (with loops)
  Inputs:
  - W: C x D array of weights
  - X: D x N array of data. Data are D-dimensional columns
  - y: 1-dimensional array of length N with labels 0...K-1, for K classes
  - reg: (float) regularization strength
  Returns:
  a tuple of:
  - loss as single float
  - gradient with respect to weights W; an array of same shape as W
  """
  dW = np.zeros(W.shape) # initialize the gradient as zero

  # compute the loss and the gradient
  num_classes = W.shape[0]
  num_train = X.shape[1]
  loss = 0.0
  for i in xrange(num_train):
    scores = W.dot(X[:, i])
    correct_class_score = scores[y[i]]
    for j in xrange(num_classes):
      if j == y[i]:
        continue
      margin = scores[j] - correct_class_score + 1 # note delta = 1
      if margin > 0:
        loss += margin
        dW[j,:] += np.transpose(X[:,i]) #for j=/= y[i]
        dW[y[i],:] -= np.transpose(X[:,i]) #rewrite the y[i] example
  # Right now the loss is a sum over all training examples, but we want it
  # to be an average instead so we divide by num_train.
  loss /= num_train
  dW/=num_train #avg grad
  # Add regularization to the loss.
  loss += 0.5 * reg * np.sum(W * W)
  dW += np.multiply(reg,W)


  return loss, dW


def svm_loss_vectorized(W, X, y, reg):
  """
  Structured SVM loss function, vectorized implementation.

  Inputs and outputs are the same as svm_loss_naive.
  """
  loss = 0.0
  dW = np.zeros(W.shape) # initialize the gradient as zero


  # Implement a vectorized version of the structured SVM loss, storing the    #
  # result in loss.                                                           #
  #############################################################################
  trains = y.shape[0]
  ypred = W.dot(X)
  estimate = ypred - ypred[list([y, range(trains)])] + 1.0
  estimate[estimate <= 0] = 0.0 #threshold the loss to 0 if we are within delta
  estimate[list([y, range(trains)])] = 0.0 #this takes care of the added delta
  loss += np.sum(estimate)/trains
  loss += 0.5 * reg * np.sum(W*W)
  
  #############################################################################
  #                             END OF YOUR CODE                              #
  #############################################################################


  #############################################################################
  # TODO:                                                                     #
  # Implement a vectorized version of the gradient for the structured SVM     #
  # loss, storing the result in dW.                                           #
  #                                                                           #
  # Hint: Instead of computing the gradient from scratch, it may be easier    #
  # to reuse some of the intermediate values that you used to compute the     #
  # loss.                                                                     #
  #############################################################################
  estimate[estimate>0] = 1.0
  dW += np.dot(estiamte,(np.transpose(X))) + reg*W
  dW/=trains
  #
  
  #############################################################################
  #                             END OF YOUR CODE                              #
  #############################################################################

  return loss, dW