set listsize 20

define pchildren
set $i = 0
set $nc = $arg0->numChildren
print $arg0->name
    while ($i < $nc)
        print $arg0->children[$i]
        set $i = $i + 1
    end
end

