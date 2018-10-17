; ModuleID = 'test.cpp'
source_filename = "test.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.14.0"

%struct.Packet = type { i8, i32 }

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @_Z8onPacket6Packeti(i64, i32) #0 {
  %3 = alloca %struct.Packet, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = bitcast %struct.Packet* %3 to i64*
  store i64 %0, i64* %6, align 4
  store i32 %1, i32* %4, align 4
  %7 = getelementptr inbounds %struct.Packet, %struct.Packet* %3, i32 0, i32 1
  %8 = load i32, i32* %7, align 4
  %9 = icmp eq i32 %8, 22
  br i1 %9, label %10, label %11

; <label>:10:                                     ; preds = %2
  store i32 5, i32* %5, align 4
  br label %11

; <label>:11:                                     ; preds = %10, %2
  ret void
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"Apple LLVM version 10.0.0 (clang-1000.11.45.2)"}
