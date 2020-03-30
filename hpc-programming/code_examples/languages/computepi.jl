
n=200000000
h=1.0/n
x=0.0
sum=0.0
pi=0.0


for i in 1:n
    x=h*(i-0.5)
    global sum=sum + (4.0/(1.0+x*x))
end
pi=h*sum
println("N iters ",n," Pi ",pi)
