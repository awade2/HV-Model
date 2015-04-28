setwd("~/Pesquisa/Sanduiche-Melian/HV-model_CPP")
# Função "wrapper" para a chamada em C:
#
# Os passos abaixo foram adaptados de http://users.stat.umn.edu/~geyer/rc/

#Sys.setenv("PKG_CPPFLAGS" = "-fopenmp -DPARALLEL") # liga biblioteca de paralelismo
system("rm HVmodel.{so,o}") #limpa sources velhos
system ("R CMD SHLIB HVmodel.cpp") ## compila no R
dyn.load("HVmodel.so") ## carrega os source resultantes como biblioteca dinamica no R

HVmodel <- function (
  N=10, 
  TRspace, 
  varianceIF=1, 
  intercImp, 
  slopeImp, 
  varianceTime=1, 
  intercTime,
  slopeTime,
  minTime=0.3,
  domination=1,
  var.IF=0,
  var.Time=0,
  cor.TrRisk=0,
  maxTime=10,
  out.code=1) 
{
  saida.C <- .C("HVmodel", 
                as.integer(N),# 1
                as.double(TRspace),# 2
                as.double(varianceIF),# 3
                as.double(intercImp),# 4
                as.double(slopeImp),# 5
                as.double(varianceTime),# 6
                as.double(intercTime),# 7
                as.double(slopeTime),# 8
                as.double(minTime),# 9
                as.integer(domination),# 10
                as.integer(var.IF),
                as.integer(var.Time),
                as.integer(cor.TrRisk),
                as.double(maxTime),# 11
                as.integer(out.code))
}

##### TESTE DO MODELO #######
TRS=c(0.1,0.25)
slp.IF=39.5/0.15
slp.Time=4.5/39.5
int.IF=0.5-0.1*slp.IF
int.Time=0.5*(1-slp.Time)

system.time(teste<-HVmodel(
  N=1000, 
  TRspace=TRS, 
  varianceIF=1, 
  intercImp=int.IF, 
  slopeImp=slp.IF, 
  varianceTime=1, 
  intercTime=int.Time,
  slopeTime=slp.Time,
  minTime=0.3,
  domination=1,
  var.IF=0,
  var.Time=0,
  cor.TrRisk=1,
  maxTime=100,
  out.code=2))
