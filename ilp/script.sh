julia="./../../julia-1.10.0/bin/julia"
optimizer="optimizer.jl"
timeout="3600"
instances="../instances/*"

if [ ! -d "results" ]; then
    mkdir "results"
fi

for file in $instances
do
  file_name=$(basename "$file")
  # echo "Optmizing $file_name ..."
  $julia $optimizer $file $timeout > "results/$file_name"
done

echo "Optimization process complete. Check results/ for the output."