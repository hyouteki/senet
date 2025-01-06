(module
  (type (;0;) (func (param i32)))
  (type (;1;) (func (param i32) (result i32)))
  (import "env" "print" (func $print (type 0)))
  (func $hash (type 1) (param i32) (result i32)
    (local i32 i32)
    local.get 0
    call $print
    block  ;; label = @1
      local.get 0
      i32.load8_u
      local.tee 1
      br_if 0 (;@1;)
      i32.const 5381
      return
    end
    local.get 0
    i32.const 1
    i32.add
    local.set 0
    i32.const 5381
    local.set 2
    loop  ;; label = @1
      local.get 2
      i32.const 33
      i32.mul
      local.get 1
      i32.const 24
      i32.shl
      i32.const 24
      i32.shr_s
      i32.add
      local.set 2
      local.get 0
      i32.load8_u
      local.set 1
      local.get 0
      i32.const 1
      i32.add
      local.set 0
      local.get 1
      br_if 0 (;@1;)
    end
    local.get 2)
  (memory (;0;) 2)
  (global $__stack_pointer (mut i32) (i32.const 66560))
  (export "memory" (memory 0))
  (export "hash" (func $hash)))
