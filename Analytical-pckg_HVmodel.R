##### Run multiple replicates of the model
multiRunHVmodel=function(PATH,nrep=100,TRS=c(0.1,0.25),VarIF=1,intIF,slIF,VarTime=1,intTime,slTime,
                        minT=0.3,DOM=1,varIFtype=0,varTimetype=0,tm=1000,ini.rep=1)
{
  oldpath <- getwd()
  dir.create(PATH, recursive=TRUE)
  setwd(PATH)
  for(i in 1:nrep)
  {
    HVmodel(N=N0, 
            TRspace=TRS, 
            varianceIF=varIF, 
            intercImp=intIF, 
            slopeImp=slIF, 
            varianceTime=varTime, 
            intercTime=intTime,
            slopeTime=slTime,
            minTime=minT,
            domination=DOM,
            var.IF=varIFtype,
            var.Time=varTimetype,
            maxTime=tm,
            out.code=ini.rep)
    ini.rep=ini.rep+1
  }
  setwd(oldpath)
}

############## Analytical Functions

General.properties.data=function(PATH="IndProp-00001.txt")
{
  read.table(PATH,,header=T,sep=" ",as.is=T)
}

# Test
IndProperties=General.properties.data("IndProp-00001.txt")
head(IndProperties)
# Some analysis
sortBytrait=IndProperties[order(IndProperties$trait),]
sortByTR=IndProperties[order(IndProperties$tolerance_risk),]
par(mfrow=c(2,1))
plot(sortBytrait$trait,sortBytrait$Simult_colaborations,xlab="Trait", ylab="Simultaneous colaborations")
plot(sortByTR$tolerance_risk,sortByTR$Simult_colaborations,xlab="Colaboration willingness", ylab="Simultaneous colaborations",col=4)
scatter3D(sortByTR$trait,sortByTR$tolerance_risk,sortByTR$Simult_colaborations,phi=10,theta=45,
          type="p",pch=19,xlab="Trait",ylab="Colaboration willingnes",zlab="Simultaneous colaborations")

plot(sortBytrait$trait,sortBytrait$Tot_colaborations,xlab="Trait", ylab="Total colaborations")
plot(sortByTR$tolerance_risk,sortByTR$Tot_colaborations,xlab="Colaboration willingness", ylab="Total colaborations",col=4)
scatter3D(sortByTR$trait,sortByTR$tolerance_risk,sortByTR$Tot_colaborations,phi=10,theta=45,
          type="p",pch=19,xlab="Trait",ylab="Colaboration willingnes",zlab="Total colaborations")
par(mfrow=c(1,1))

################ 

teste=read.table("outTIMES-00001.txt",col.names = rep("",max(IndProperties$Tot_colaborations)),fill = T,
                 as.is=T,sep=" ")
teste2=read.table("outCOLAB-00001.txt",col.names = rep("",max(IndProperties$Tot_colaborations)),fill = T,
                 as.is=T,sep=" ")
teste3=read.table("outIFac-00001.txt",col.names = rep("",max(IndProperties$Tot_colaborations)),fill = T,
                 as.is=T,sep=" ")

data.table=function(TIMES,colabs,IF,prop.inds,N=1000)
{
  Nrows=sum(prop.inds$Tot_colaborations)
  tms=numeric(Nrows)
  colbs=numeric(Nrows)
  ImpFac=numeric(Nrows)
  ids=numeric(Nrows)
  traits=numeric(Nrows)
  tolRisk=numeric(Nrows)
  pini=1
  pend=0
  no.na=which(prop.inds$Tot_colaborations==max(prop.inds$Tot_colaborations))
  for (i in 1:(no.na-1))
  {
    pend=pend+prop.inds$Tot_colaborations[i]
    columns=which(is.na(TIMES[i,]))
    tms[pini:pend]=as.numeric(TIMES[i,-columns])
    colbs[pini:pend]=as.numeric(colabs[i,-columns])
    ImpFac[pini:pend]=as.numeric(IF[i,-columns])
    ids[pini:pend]=rep(i,prop.inds$Tot_colaborations[i])
    traits[pini:pend]=rep(prop.inds[i,2],prop.inds$Tot_colaborations[i])
    tolRisk[pini:pend]=rep(prop.inds[i,3],prop.inds$Tot_colaborations[i]) 
    pini=pend+1
  }
  pend=pend+prop.inds$Tot_colaborations[no.na]
  tms[pini:pend]=as.numeric(TIMES[no.na,])
  colbs[pini:pend]=as.numeric(colabs[no.na,])
  ImpFac[pini:pend]=as.numeric(IF[no.na,])
  ids[pini:pend]=rep(no.na,prop.inds$Tot_colaborations[no.na])
  traits[pini:pend]=rep(prop.inds[no.na,2],prop.inds$Tot_colaborations[no.na])
  tolRisk[pini:pend]=rep(prop.inds[no.na,3],prop.inds$Tot_colaborations[no.na]) 
  pini=pend+1
  for (i in (no.na+1):N)
  {
    pend=pend+prop.inds$Tot_colaborations[i]
    columns=which(is.na(TIMES[i,]))
    tms[pini:pend]=as.numeric(TIMES[i,-columns])
    colbs[pini:pend]=as.numeric(colabs[i,-columns])
    ImpFac[pini:pend]=as.numeric(IF[i,-columns])
    ids[pini:pend]=rep(i,prop.inds$Tot_colaborations[i])
    traits[pini:pend]=rep(prop.inds[i,2],prop.inds$Tot_colaborations[i])
    tolRisk[pini:pend]=rep(prop.inds[i,3],prop.inds$Tot_colaborations[i]) 
    pini=pend+1
  }
tab=data.frame(Times=tms,ID=ids,colaborator=colbs,IF=ImpFac,trait_id=traits,TR_id=tolRisk)
return(tab)
}
##### Test data.table
SIM.results=data.table(TIMES = teste,colabs = teste2,IF = teste3,prop.inds = IndProperties)


trend.analysis=function(dat,prop.inds,tms=100,N=1000,intervals=1)
{
  trend.data=data.frame(ID=numeric(tms*N),
                        Trait=numeric(tms*N),
                        TR=numeric(tms*N),
                        pubRate=numeric(tms*N),
                        pubScore1=numeric(tms*N),
                        pubScore2=numeric(tms*N),
                        pubScore3=numeric(tms*N))
  pini=1
  pend=0
  for(i in 1:tms)
  {
    if(length(unique(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]))<N)
    {
      response.vars=data.frame(ID=as.double(levels(as.data.frame(table(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]))[,1])),
                               NPub=as.numeric(table(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)])),
                               score1=aggregate(dat$IF[which(dat$Times>=(i-intervals) & dat$Times<i)],list(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]),
                                                sum)[,2],
                               score2=aggregate(dat$IF[which(dat$Times>=(i-intervals) & dat$Times<i)],list(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]),
                                                mean)[,2],
                               score3=aggregate(dat$IF[which(dat$Times>=(i-intervals) & dat$Times<i)],list(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]),
                                                var)[,2])
      toMerge=data.frame(ID=prop.inds$id)
      resp.var.comp=merge(toMerge,response.vars,all.x=T)
      resp.var.comp[is.na(resp.var.comp[,2]),-1]=0
    }
    else
    {
      resp.var.comp=data.frame(ID=1:N,
                               NPub=as.numeric(table(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)])),
                               score1=aggregate(dat$IF[which(dat$Times>=(i-intervals) & dat$Times<i)],list(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]),
                                                sum)[,2],
                               score2=aggregate(dat$IF[which(dat$Times>=(i-intervals) & dat$Times<i)],list(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]),
                                                mean)[,2],
                               score3=aggregate(dat$IF[which(dat$Times>=(i-intervals) & dat$Times<i)],list(dat$ID[which(dat$Times>=(i-intervals) & dat$Times<i)]),
                                                var)[,2])
    }
    pend=pend+N
    trend.data[pini:pend,c(1,4:7)]=resp.var.comp
    trend.data[pini:pend,2:3]=c(prop.inds$trait,prop.inds$tolerance_risk)    
    pini=pend+1
  }
return(trend.data)  
}
##### Test trend,table
trend.table=trend.analysis(dat=SIM.results,prop.inds=IndProperties)
trend.table$pubScore3[which(is.na(trend.table$pubScore3))]=0
trend.table2=trend.analysis(dat=SIM.results,prop.inds=IndProperties,intervals=5)
trend.table2$pubScore3[which(is.na(trend.table2$pubScore3))]=0
#head(trend.table.ord)
trend.table.ord=trend.table[order(trend.table[,2]),]
trend.table.ord2=trend.table2[order(trend.table2[,2]),]
zzz=matrix(trend.table.ord$pubScore2,100,1000)
zzz2=matrix(trend.table.ord$pubScore3,1000,100,byrow = T)
image(1:100,IndProperties$trait[order(IndProperties$trait)],zzz,col=rainbow(12),xlab="Time",ylab="Trait", main="Impact Factor mean")
image(IndProperties$trait[order(IndProperties$trait)],1:100,zzz2,col=rainbow(max(trend.table.ord$pubRate)))
scatter3D(trend.table$Trait,rep(1:100,each=1000),trend.table$pubRate,phi=10,theta=45,
          type="p",pch=19,xlab="Trait",ylab="Time",zlab="Publication Rate")
scatter3D(rep(1:100,each=1000),trend.table$Trait,trend.table$pubScore1,phi=45,theta=45,
          type="h",pch=19,xlab="Time",ylab="Trait",zlab="Impact Factor (sum)",cex=0.2)


plot(1:100,zzz[,10],type="l", ylim=c(0,120))
lines(1:100,zzz[,300],type="l",col=5)

head(SIM.results)


undebug(trend.analysis)
?merge

qqq=rep(1:10,10)
aaa=rep(1:10,each=10)

aaa

data.table

qqq=SIM.results[order(SIM.results$Times),]
nnn=aggregate(qqq$IF[which(qqq$Times>=0 & qqq$Times<1)],list(qqq$ID[which(qqq$Times>=0 & qqq$Times<1)]),
              sum)
levels(table(qqq$ID[which(qqq$Times>=0 & qqq$Times<1)]))
sss=as.numeric(table(qqq$ID[which(qqq$Times>=0 & qqq$Times<1)]))
ccc=as.double(levels(as.data.frame(table(qqq$ID[which(qqq$Times>=0 & qqq$Times<1)]))[,1]))
ppp=data.frame(Var1=ccc,NPub=sss)
names(ccc)="Var1"
vvv=data.frame(Var1=IndProperties$id)
ttt=merge(vvv,ppp,all.x=T)
ttt[is.na(ttt[,2]),2]=0

length(unique(qqq$ID[which(qqq$Times>=18 & qqq$Times<19)]))

qqq=numeric()
qqq=c(qqq,1:10)
qqq
which(IndProperties$Tot_colaborations==max(IndProperties$Tot_colaborations))

oldpar=par("mar")
par(mar=oldpar)
t1=as.numeric(teste[599,-which(is.na(teste[599,]))])
t2=as.numeric(teste3[599,-which(is.na(teste3[599,]))])
plot(t1,t2)
IndProperties[994,]
sss=teste2[415,]
names(sss)=rep(".",length(sss))
sss=as.numeric(sss)
?print
cat(sss)
options()$max.print=99999
length(sss)
dim(teste2)
length(as.numeric(teste2[416,]))
as.numeric(teste2[416,])==as.numeric(teste2[415,])
IndProperties