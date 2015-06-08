library(lattice)

ifactor<-read.csv("./outIFac-00002.txt",sep=" ",header=FALSE)
nrows<-length(ifactor[,1])#1060
ncols<-length(ifactor[1,])#13340
grid<-expand.grid(x=1:1060,y=1:13340)
grid$z<-as.numeric(as.matrix(ifactor))

postscript("./IF_distribution.eps",width=1024,height=800)
hist(as.numeric(as.matrix(ifactor)),xlab="IF",ylab="Freq",main="Impact Factor Distribution",nclass=100,col="red",xlim=c(0,50),cex=1.5)
dev.off()
