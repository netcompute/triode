
struct Packet {
  char flag;
  int dst;
};

void onPacket(struct Packet pkt, int b) {
  if (pkt.dst == 22) {
    int a = 5;
  }
}