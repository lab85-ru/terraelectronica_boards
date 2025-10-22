/**
  @page DMA2D_MemToMemWithBlending DMA2D memory to memory with blending example
  
  @verbatim
  ******************** (C) COPYRIGHT 2013 STMicroelectronics *******************
  * @file    DMA2D/DMA2D_MemToMemWithBlending/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0RC2
  * @date    20-February-2013
  * @brief   Description of the STM32F4xx DMA2D_MemToMemWithBlending example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  @endverbatim

@par Example Description

  This example provides a description of how to configure DMA2D peripheral in 
  Memory_to_Memory with blending transfer mode.
 
  In this transfer mode two input sources are fitched : foreground and background.
  In this example, the foreground and background are configured as following :
   - input memory address at FLASH memory 
   - Color mode : RGB565 format
   - The constant alpha for foreground is decreased to see the background.
   - The alpha mode for foreground and background is configured to see two 
     superposed images in the resulting image.
        
  The DMA2D blends the two sources pixels to compute the resulting pixel. 
  The transferred data to the output memory address is the result of the blending 
  operation.
 
  In this example, the layer 1 is configured to display the DMA2D output buffer. 
  (the result of the blending of the foreground and background)      

@par Directory contents

    - DMA2D/DMA2D_MemToMemWithBlending/main.h               Main configuration file
    - DMA2D/DMA2D_MemToMemWithBlending/stm32f4xx_it.h       Header for stm32f4xx_it.c
    - DMA2D/DMA2D_MemToMemWithBlending/stm32f4xx_conf.h     library configuration file 
    - DMA2D/DMA2D_MemToMemWithBlending/RGB565_230x170_1.h   image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/RGB565_230x170_2.h   image used for DMAD2D validation
    - DMA2D/DMA2D_MemToMemWithBlending/main.c               Main program  
    - DMA2D/DMA2D_MemToMemWithBlending/stm32f4xx_it.c       Interrupt handlers
    - DMA2D/DMA2D_MemToMemWithBlending/system_stm32f4xx.c   STM32F4xx system clock configuration file


@par Hardware and Software environment  
  - This example runs on STM32F429x/STM32F439x Devices. 
   
  - This example has been tested with STMicroelectronics STM324x9I-EVAL (STM32F429x/ 
    STM32F439x Devices) evaluation boards and can be easily tailored to any other 
    supported device and development board. 


@par How to use it ?
In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F4xx_StdPeriph_Templates
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   