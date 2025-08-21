#pragma once
#include "./matching_engine.h"

namespace exch_ns
{
class MatchingEngineOrchestrator
{
    static int INSTANCE_COUNT;

  public:
    MatchingEngineOrchestrator();

    void Start();
    void Stop();
    void OnNewOrder();
    void OnModifyOrder();
    void OnCancelOrder();

  private:
    MatchingEngine engine_;
    int            instance_id_ = 0;
};
} // namespace exch_ns