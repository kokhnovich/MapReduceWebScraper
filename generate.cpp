#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

std::vector<std::string> data = {
    "youtube.com",
    "en.wikipedia.org",
    "twitter.com",
    "facebook.com",
    "amazon.com",
    "yelp.com",
    "reddit.com",
    "imdb.com",
    "fandom.com",
    "pinterest.com",
    "tripadvisor.com",
    "instagram.com",
    "walmart.com",
    "craigslist.org",
    "ebay.com",
    "linkedin.com",
    "play.google.com",
    "healthline.com",
    "etsy.com",
    "indeed.com",
    "apple.com",
    "espn.com",
    "webmd.com",
    "fb.com",
    "nytimes.com",
    "google.com",
    "cnn.com",
    "merriam-webster.com",
    "gamepedia.com",
    "microsoft.com",
    "target.com",
    "homedepot.com",
    "quora.com",
    "nih.gov",
    "rottentomatoes.com",
    "netflix.com",
    "quizlet.com",
    "weather.com",
    "mapquest.com",
    "britannica.com",
    "businessinsider.com",
    "dictionary.com",
    "zillow.com",
    "mayoclinic.org",
    "bestbuy.com",
    "theguardian.com",
    "yahoo.com",
    "msn.com",
    "usatoday.com",
    "medicalnewstoday.com",
    "urbandictionary.com",
    "usnews.com",
    "foxnews.com",
    "genius.com",
    "allrecipes.com",
    "spotify.com",
    "glassdoor.com",
    "forbes.com",
    "cnet.com",
    "finance.yahoo.com",
    "irs.gov",
    "lowes.com",
    "mail.yahoo.com",
    "aol.com",
    "steampowered.com",
    "washingtonpost.com",
    "usps.com",
    "office.com",
    "retailmenot.com",
    "wiktionary.org",
    "paypal.com",
    "foodnetwork.com",
    "hulu.com",
    "live.com",
    "cbssports.com",
    "wayfair.com",
    "ca.gov",
    "bleacherreport.com",
    "macys.com",
    "accuweather.com",
    "xfinity.com",
    "go.com",
    "techradar.com",
    "groupon.com",
    "investopedia.com",
    "yellowpages.com",
    "steamcommunity.com",
    "chase.com",
    "wellsfargo.com",
    "npr.org",
    "apartments.com",
    "roblox.com",
    "huffpost.com",
    "books.google.com",
    "bankofamerica.com",
    "bbb.org",
    "expedia.com",
    "wikihow.com",
    "ign.com",
    "wowhead.com"
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "There should be 1 argument - number of websites" << std::endl;
    return 1;
  }
  int count = atoi(argv[1]);
  if (count > data.size()) {
    std::cerr << "There is no possibility to generate such a huge dataset" << std::endl;
    return 1;
  }
  std::shuffle(data.begin(), data.end(), std::mt19937(std::random_device()()));
  for (int i = 0; i < count; ++i) {
    std::cout << "https://" << data[i] << "\t1" << std::endl;
  }
  return 0;
}
