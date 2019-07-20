
#ifndef _TERMUTEST_H_
#define _TERMUTEST_H_

#define PRINT_RESULT(a) if(a) {printf("%-40s:%4d \t\t PASS\n", __func__, __LINE__);}else{result = false;printf("%-40s:%4d \t\t FAIL\n", __func__, __LINE__);}


#endif

