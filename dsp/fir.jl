# include("fir.jl")
# import Pkg; Pkg.add("DSP")
import DSP

a1 = read(expanduser("~/Music/sheep-16k.raw"))
a2 = Array{Float64}(a1)

h = ones(20)
for i = 1:length(h)
	h[i] = i
end
# h = ones(5)
h = h/sum(h)
h = reverse(h)

a3 = DSP.conv(a2, h)
a4 = round.(UInt8, a3)
write("out-fir.raw", a4)

