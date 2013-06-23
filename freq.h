/*\
 *  This is an awesome programm simulating awesome battles of awesome robot tanks
 *  Copyright (C) 2013  Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

#ifndef FREQ_H
#define FREQ_H

/* From https://en.wikipedia.org/wiki/Letter_frequency

The original statistics actually comports diacritics, so the results are
not precise.

For French:
  * 'à' is counted as 'a;
  * 'ç' as 'c'.
  * 'è', 'é', 'ê' and 'ë' as 'e';
  * 'î' and 'ï' as 'i';
  * 'û' as 'u'.

English  French German Spanish Portuguese Esperanto Italian Turkish Swedish Polish  Dutch
 8.167%  8.122%  6.51%  12.53%     14.63%    12.12%  11.74%  11.68%    9.3%   8.0%  7.49%
 1.492%  0.901%  1.89%   1.42%      1.04%     0.98%   0.92%   2.95%    1.3%   1.3%  1.58%
 2.782%  3.345%  3.06%   4.68%      3.88%     0.78%   4.5%    0.97%    1.3%   3.8%  1.24%
 4.253%  3.669%  5.08%   5.86%      4.99%     3.04%   3.73%   4.87%    4.5%   3.0%  5.93%
12.702% 14.715% 17.40%  13.68%     12.57%     8.99%  11.79%   9.01%    9.9%   6.9% 18.91%
 2.228%  1.066%  1.66%   0.69%      1.02%     1.03%   0.95%   0.44%    2.0%   0.1%  0.81%
 2.015%  0.866%  3.01%   1.01%      1.30%     1.17%   1.64%   1.34%    3.3%   1.0%  3.40%
 6.094%  0.737%  4.76%   0.70%      1.28%     0.38%   1.54%   1.14%    2.1%   1.0%  2.38%
 6.966%  7.579%  7.55%   6.25%      6.18%    10.01%  11.28%   8.27%*   5.1%   7.0%  6.50%
 0.153%  0.545%  0.27%   0.44%      0.40%     3.50%   0.00%   0.01%    0.7%   1.9%  1.46%
 0.772%  0.049%  1.21%   0.01%      0.02%     4.16%   0.00%   4.71%    3.2%   2.7%  2.25%
 4.025%  5.456%  3.44%   4.97%      2.78%     6.14%   6.51%   5.75%    5.2%   3.1%  3.57%
 2.406%  2.968%  2.53%   3.15%      4.74%     2.99%   2.51%   3.74%    3.5%   2.4%  2.21%
 6.749%  7.095%  9.78%   6.71%      5.05%     7.96%   6.88%   7.23%    8.8%   4.7% 10.03%
 7.507%  5.378%  2.51%   8.68%     10.73%     8.78%   9.83%   2.45%    4.1%   7.1%  6.06%
 1.929%  3.021%  0.79%   2.51%      2.52%     2.74%   3.05%   0.79%    1.7%   2.4%  1.57%
 0.095%  1.362%  0.02%   0.88%      1.20%     0.00%   0.51%      0%  0.007%      - 0.009%
 5.987%  6.553%  7.00%   6.87%      6.53%     5.91%   6.37%   6.95%    8.3%   3.5%  6.41%
 6.327%  7.948%  7.27%   7.98%      7.81%     6.09%   4.98%   2.95%    6.3%   3.8%  3.73%
 9.056%  7.244%  6.15%   4.63%      4.74%     5.27%   5.62%   3.09%    8.7%   2.4%  6.79%
 2.758%  6.369%  4.35%   3.93%      4.63%     3.18%   3.01%   3.43%    1.8%   1.8%  1.99%
 0.978%  1.628%  0.67%   0.90%      1.67%     1.90%   2.10%   0.98%    2.4%      -  2.85%
 2.360%  0.114%  1.89%   0.02%      0.01%     0.00%   0.00%      0%   0.03%   3.6%  1.52%
 0.150%  0.387%  0.03%   0.22%      0.21%     0.00%   0.00%      0%    0.1%      -  0.04%
 1.974%  0.308%  0.04%   0.90%      0.01%     0.00%   0.00%   3.37%    0.6%   3.2% 0.035%
 0.074%  0.136%  1.13%   0.52%      0.47%     0.50%   0.49%   1.50%   0.02%   5.1%  1.39%
*/

static const float english[26] =
{
	0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094,
	0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749, 0.07507, 0.01929,
	0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
	0.01974, 0.00074,
};

static const float french[26] =
{
	0.08122, 0.00901, 0.03345, 0.03669, 0.14715, 0.01066, 0.00866, 0.00737,
	0.07579, 0.00545, 0.00049, 0.05456, 0.02968, 0.07095, 0.05378, 0.03021,
	0.01362, 0.06553, 0.07948, 0.07244, 0.06369, 0.01628, 0.00114, 0.00387,
	0.00308, 0.00136,
};

#endif
