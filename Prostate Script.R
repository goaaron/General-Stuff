#Prostate Data

library(faraway)
data(prostate)
attach(prostate)
lm1 <- lm(lpsa~lcavol, data=prostate)
lm2 <- lm(lpsa~lcavol + lweight, data = prostate)
lm3 <- lm(lpsa~lcavol + lweight + svi, data = prostate)
lm4 <- lm(lpsa~lcavol + lweight + svi + lbph, data = prostate)
lm5 <- lm(lpsa~lcavol + lweight + svi + lbph + age, data = prostate)
lm6 <- lm(lpsa~lcavol + lweight + svi + lbph + age + lcp, data = prostate)
lm7 <- lm(lpsa~lcavol + lweight + svi + lbph + age + lcp + pgg45, data = prostate)
lm8 <- lm(lpsa~lcavol + lweight + svi + lbph + age + lcp + pgg45 + gleason, data = prostate)
rsquareds <- c(summary(lm1)$r.squared, summary(lm2)$r.squared,summary(lm3)$r.squared,summary(lm4)$r.squared,summary(lm5)$r.squared,
               summary(lm6)$r.squared,summary(lm7)$r.squared,summary(lm8)$r.squared)

rse <- c(summary(lm1)$sigma, summary(lm2)$sigma,summary(lm3)$sigma,summary(lm4)$sigma,summary(lm5)$sigma,
         summary(lm6)$sigma,summary(lm7)$sigma,summary(lm8)$sigma)
plot(1:8,rse,xlab="features",ylab="RSE",main="RSE vs Features")
plot(1:8,rsquareds,xlab="features",ylab="R2",main="R2 vs Features")


#
plot(lcavol,lpsa, xlab="lcavol",ylab="lpsa", main="Lpsa vs Lcavol")
lm9 <- lm(lpsa~lcavol, data=prostate)
lm10 <- lm(lcavol~lpsa, data=prostate)
abline(lm9)
abline(lm10)