/*Stats computeStats(const std::vector<double>& data)
{
    Stats s;
    s.count = data.size();
    if (s.count == 0) return s;

    s.sum = std::accumulate(data.begin(), data.end(), 0.0);
    s.mean = s.sum / static_cast<double>(s.count);

    auto mm = std::minmax_element(data.begin(), data.end());
    s.min = *mm.first;
    s.max = *mm.second;

    double accum = 0.0;
    for (double v : data)
    {
        double d = v - s.mean;
        accum += d * d;
    }

    s.variance = accum / static_cast<double>(s.count); // population variance
    s.stddev = std::sqrt(s.variance);
    return s;
}*/