
for line in `awk -F' ' '{print $4}' $1`;do
  egrep "$line" ../stop_times.txt >/dev/null
  if [ ! $? -eq 0 ]; then
      echo $line
  fi
done
