#include "export.hpp"

#include <sstream>

#include "common.hpp"
#include "game.hpp"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace fs = std::filesystem;

namespace json {

using namespace rapidjson;

rapidjson::Value FromStr(const std::string& str,
                         Document::AllocatorType& alloc) {
  return Value{str.c_str(), alloc};
}

rapidjson::Value FromStrView(const std::string_view& str,
                             Document::AllocatorType& alloc) {
  return Value{str.data() ? str.data() : "", alloc};
}

}  // namespace json

Export::Export(fs::path dir) {
  using std::ios;

  fs::create_directories(dir);
  dir = fs::canonical(dir);
  LOGI("Simulation output dir {}", dir.c_str());

  json_out_.open(dir / "simulation.json", ios::out | ios::trunc);

  json_.SetObject();
};

Export::~Export() {
  json::StringBuffer s;
  json::Writer<json::StringBuffer> writer(s);
  json_.Accept(writer);
  json_out_ << s.GetString();

  json_out_.close();
}

void Export::DumpGameParams(const GameParams& params) {
  if (json_.HasMember("params")) {
    LOGW("JSON params has been already dumped");
    return;
  }
  json::Value s(json::kObjectType);
  s.AddMember("balance", params.Balance(), Alloc());
  s.AddMember("deck_size", params.DeckSize(), Alloc());
  s.AddMember("pool_size", params.PoolSize(), Alloc());
  s.AddMember("cards", params.StartingCards(), Alloc());
  s.AddMember("turn_limit", params.TurnLimit(), Alloc());
  s.AddMember("leading", params.PointsLeading(), Alloc());
  json_.AddMember("params", s, Alloc());
}

void Export::DumpFinalResult(const GameResult& result) {
  if (json_.HasMember("status")) {
    LOGW("JSON result has been already dumped");
    return;
  }
  {
    json::Value status(json::kObjectType);
    status.AddMember("code", result.Status(), Alloc());
    status.AddMember(
        "msg", json::FromStrView(result.StatusStringify(), Alloc()), Alloc());
    json_.AddMember("status", status, Alloc());
  }
  if (!result.Ok()) return;
  json::Value players[2];
  for (int i = 0; i < 2; ++i) {
    auto& final_result = result.FinalResult()[i];
    players[i].SetObject();
    players[i].AddMember("name", json::FromStrView(final_result.name, Alloc()),
                         Alloc());
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << final_result.strength;
    players[i].AddMember("strength", json::FromStr(ss.str(), Alloc()), Alloc());
  }
  switch (result.Status()) {
    case GameResult::kDone:
      json_.AddMember("winner", players[0], Alloc());
      json_.AddMember("loser", players[1], Alloc());
      break;
    case GameResult::kDraw: {
      json::Value drawers(json::kArrayType);
      drawers.PushBack(players[0], Alloc());
      drawers.PushBack(players[1], Alloc());
      json_.AddMember("drawers", drawers, Alloc());
      break;
    }
    default:
      break;
  }
}
