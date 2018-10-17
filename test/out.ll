; ModuleID = 'main'
source_filename = "main"

%Packet = type { i32 }

declare void @drop()

define void @onPacket(%Packet) {
entry:
  %1 = alloca %Packet
  store %Packet %0, %Packet* %1
  %memberPtr = getelementptr inbounds %Packet, %Packet* %1, i32 0, i32 0
  %2 = load i32, i32* %memberPtr
  %cmptmp = icmp eq i32 %2, 22
  br i1 %cmptmp, label %then, label %ifcont

then:                                             ; preds = %entry
  %3 = load %Packet, %Packet* %1
  %calltmp = call void @drop(%Packet %3)
  br label %ifcont

ifcont:                                           ; preds = %then, %entry
}
