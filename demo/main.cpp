#include <header.hpp>

int main(int argc, char *argv[]) {
    signal(SIGINT, Hasher::Catch_Signal);
    Hasher hasher(argc, argv);
    hasher.Start(true);
}