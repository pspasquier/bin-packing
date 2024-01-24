using JuMP, GLPK

f = open(ARGS[1], "r");
timeout = parse(Int, ARGS[2]);
N = parse(Int, readline(f));
C = parse(Int, readline(f));
P = [parse(Int, readline(f)) for _ in 1:N];
   
model = Model(GLPK.Optimizer);
@variable(model, x[1:N], Bin);
@variable(model, y[1:N, 1:N], Bin);
@objective(model, Min, sum(x));
@constraints(model, begin
    [i=1:N], sum(P[j]*y[i,j] for j=1:N) <= C*x[i]
    [j=1:N], sum(y[i,j] for i=1:N) == 1
end);
set_time_limit_sec(model, timeout);
optimize!(model);

println(solution_summary(model));