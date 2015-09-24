library(faraway)
data(prostate)
l1 <- lm(lpsa ~ ., data=prostate)
##
#CI 90%
co <- summary(m0)$coefficients
c901<- co[4, 1] - qt(0.95, 88) * co[4, 2]
c902<- co[4, 1] + qt(0.95, 88) * co[4, 2]


# CI 95%
co[4, 1] - qt(0.975, 88) * co[4, 2]
co[4, 1] + qt(0.975, 88) * co[4, 2]

# Only 95% contains 0 so p-value for age is between .05-.1 ... not necessarily significant

## joint confidence region
library(ellipse)
plot(ellipse(l1, c(4,5)))
points(summary(l1)$coefficients[4], summary(l1)$coefficients[5])
# origin in ellipse, can't reject the null hypothesis with this 95% hoint confidence.

##
patient <- data.frame(lcavol=1.44682, lweight=3.62301, age=65.00000, lbph=.30010, svi=0.00000, lcp=-.79851, gleason=7.0000, pgg45=15.000)
confint(l1, level=.95)[4,]
prediction <- predict(l1, patient, interval="prediction")
#       fit       lwr      upr
#1 2.388994 0.9646004 3.813388

##
patient2 <-data.frame(lcavol=1.44682, lweight=3.62301, age=20.00000, lbph=.30010, svi=0.00000, lcp=-.79851, gleason=7.0000, pgg45=15.000)
predict(l1, patient2, interval="prediction")
#      fit      lwr      upr
#1 3.272667 1.538688 5.006646
#wider confidence interval because age 20 is far from observed age range