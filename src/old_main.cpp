  /* while (run) { */
  /*   std::string input; */
  /*   std::cout << "welcome to bit blitzer:" << std::endl; */
  /*   std::cout << "-----------------------" << std::endl; */
  /*   std::cout << "commands:" << std::endl; */
  /*   std::cout << "- play" << std::endl; */
  /*   std::cout << "- eval" << std::endl; */
  /*   std::cout << "- perft" << std::endl; */
  /*   std::cout << "- exit" << std::endl; */
  /*   std::cout << "-----------------------" << std::endl; */
  /*   std::cout << ">> "; */
  /*   std::cin >> input; */
  /*   std::cout << "-----------------------" << std::endl; */

  /*   if (input == "play") { */
  /*     bool play = true; */
  /*     fenparser.loadstartingposition(); */
  /*     while (play) { */
  /*       bitboard::printbb(board.getallpieces()); */
  /*       std::string start, target; */
  /*       std::cout << "input move: " << std::endl; */
  /*       std::cin >> start >> target; */

  /*       if (start == "q") { */
  /*         play = false; */
  /*         continue; */
  /*       } */

  /*       std::vector<int> moves; */
  /*       bool incheck = position.getwhitetomove() ? position.whiteincheck */
  /*                                                : position.blackincheck; */
  /*       movegenerator.generatemoves(moves, position.getwhitetomove(), */
  /*                                   position.getcastlingrighs(), */
  /*                                   position.getenpassantsquare(), incheck);
   */

  /*       for (int &move : moves) { */
  /*         if (bitboard::indextocoordinate(getmovestart(move)) == start && */
  /*             bitboard::indextocoordinate(getmovetarget(move)) == target) {
   */
  /*           position.makemove(move); */
  /*           bitboard::printbb(board.getallpieces()); */
  /*           break; */
  /*         } */
  /*       } */

  /*       std::cout << "thinking..." << std::endl; */
  /*       int robotmove = search.search(5); */
  /*       position.makemove(robotmove); */
  /*     } */
  /*   } */

  /*   if (input == "eval") { */
  /*     // get position and depth */
  /*     std::string fen; */
  /*     std::cout << "input fen string (\"startpos\" for starting position): ";
   */
  /*     std::cin.ignore(); */
  /*     std::getline(std::cin, fen); */
  /*     if (fen == "startpos") { */
  /*       fenparser.loadstartingposition(); */
  /*     } else { */
  /*       fenparser.loadfenposition( */
  /*           "rnbqkbnr/1pppppp1/p6p/8/2b1p3/5q2/pppp1ppp/rnb1k1nr b kqkq - 1
   * 3"); */
  /*     } */
  /*     int depth; */
  /*     std::cout << "input depth: "; */
  /*     std::cin >> depth; */

  /*     // run evaluation */
  /*     std::cout << "\nalpha beta test at depth " << depth << std::endl; */
  /*     search.negamaxtest(depth); */
  /*   } */

  /*   if (input == "perft") { */
  /*     std::string fen = "8/2p5/3p4/kp5r/1r3p1k/8/4p1p1/8 w - - 0"; */
  /*     fenparser.loadfenposition(fen); */
  /*     int depth; */
  /*     std::cout << "input depth: "; */
  /*     std::cin >> depth; */
  /*     std::cout << "\nperft at depth: " << depth << std::endl; */
  /*     perft.perft(depth); */
  /*   } */

  /*   if (input == "exit") { */
  /*     run = false; */
  /*   } */
  /* } */
  /* std::cout << "shutting down" << std::endl; */
  /* } */
