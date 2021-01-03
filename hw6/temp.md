

## Part.c



Following is how podem algorithm works



For a givne fault f, there are 3 possibilities for that fault, TRUE, CONFLICT, FALSE. The function `podem` will return one of the statuses.



```
func Podem()
	set all gate values to unknown (x)
  MarkPropagateTree
  status = SetUniqueImpliedValue
  
```





```

```



```

//inject fault-free value and do simple backward implication
//TRUE: fault can be backward propagated to PI
//CONFLICT: conflict assignment
//FALSE: fault can not be propagated to PI

ATPG_STATUS CIRCUIT::SetUniqueImpliedValue(FAULT* fptr)


//apply the input values of gate according to its output value
//TRUE: fault can be backward propagated to PI
//CONFLICT: conflict assignment
//FALSE: fault can not be propagated to PI
ATPG_STATUS CIRCUIT::BackwardImply(GATEPTR gptr, VALUE value)


//mark and push propagate tree to stack PropagateTree
void CIRCUIT::MarkPropagateTree(GATEPTR gptr)


//fault injection
//true: fault is injected successfully and need to do fault propagation
//false: output value is the same with original one or fault is injected in PO
bool CIRCUIT::FaultEvaluate(FAULT* fptr)


//return possible PI decision
GATEPTR CIRCUIT::TestPossible(FAULT* fptr)

//find PI decision to set gptr = value
//success: return PI
//fail: return 0
GATEPTR CIRCUIT::FindPIAssignment(GATEPTR gptr, VALUE value)


//check if the fault has propagated to PO
bool CIRCUIT::CheckTest()


//search gate from propagate tree to propagate the fault
GATEPTR CIRCUIT::FindPropagateGate()

//trace if any unknown path from gptr to PO
bool CIRCUIT::TraceUnknownPath(GATEPTR gptr)

```



fault activation, fault propagation, backtrace, forward simulation, backtrack